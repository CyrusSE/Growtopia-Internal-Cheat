#include "VariantDB.h"
#include "PlatformPrecomp.h"

//YEETED from resourceutils.cpp
bool LoadFromFile(string& str, FILE* fp) {
#ifdef RT_FORCE_32BIT_INTS_FOR_FILES
    int32 size;
    fread(&size, sizeof(int32), 1, fp);

#else
    int size;
    fread(&size, sizeof(int), 1, fp);
#endif

    if (size > 0) {
        str.resize(size, ' ');
        fread(&str[0], size, 1, fp);
    }
    else {
        str.clear();
    }
    return true;
}
bool LoadFromFile(int& num, FILE* fp) {
    fread(&num, sizeof(int), 1, fp);
    return true;
}

bool LoadFromFile(float& num, FILE* fp) {
    fread(&num, sizeof(float), 1, fp);
    return true;
}

bool LoadFromFile(bool& num, FILE* fp) {
    fread(&num, sizeof(bool), 1, fp);
    return true;
}
bool LoadFromFile(unsigned int& num, FILE* fp) {
    fread(&num, sizeof(unsigned int), 1, fp);
    return true;
}

bool LoadFromFile(CL_Vec2f& num, FILE* fp) {
    fread(&num, sizeof(CL_Vec2f), 1, fp);
    return true;
}

bool LoadFromFile(CL_Vec3f& num, FILE* fp) {
    fread(&num, sizeof(CL_Vec3f), 1, fp);
    return true;
}

bool LoadFromFile(CL_Rectf& num, FILE* fp) {
    fread(&num, sizeof(CL_Rectf), 1, fp);
    return true;
}

VariantDB::VariantDB() {
    ResetNext();
}

VariantDB::~VariantDB() {
    DeleteAll();
}

void VariantDB::DeleteAll() {
    dataList::iterator itor = m_data.begin();
    while (itor != m_data.end()) {
        SAFE_DELETE(itor->second);
        itor++;
    }

    m_data.clear();
}

int VariantDB::DeleteVar(string keyName) {
    int deleted = 0;

    dataList::iterator itor = m_data.begin();

    while (itor != m_data.end()) {
        if (itor->first == keyName) {
            //match!
            delete (itor->second);
            dataList::iterator itorTemp = itor;
            itor++;

            m_data.erase(itorTemp);
            deleted++;
            continue;
        }
        itor++;
    }

    return deleted;
}

void VariantDB::SetVarGhetto(string name, Variant val) { //cuz GetVar crashes for whatever reason
    dataList::iterator itor = m_data.begin();
    while (itor != m_data.end()) {
        Variant* pV = itor->second;
        if (itor->first == name) {
            pV->Set(val);
            break;
        }
          

         itor++;
    }
}

Variant* VariantDB::GetVarIfExists(const string& keyName) {
    dataList::iterator itor = m_data.find(keyName);

    if (itor != m_data.end()) {
        //bingo!
        return &((*itor->second));
    }

    return NULL;
}

Variant* VariantDB::GetVarWithDefault(const string& keyName, const Variant& vDefault) {
    Variant* pData = GetVarIfExists(keyName);

    if (!pData) {
        //create it
        pData = new Variant(vDefault);
        m_data[keyName] = pData;
    }

    return pData;
}

Variant* VariantDB::GetVar(const string& keyName) {
    Variant* pData = GetVarIfExists(keyName);

    if (!pData) {
        //create it
        pData = new Variant;
        m_data[keyName] = pData;
    }

    return pData;
}

void VariantDB::CallFunctionIfExists(const string& keyName, VariantList* pVList) {
}

bool VariantDB::Save(const string& fileName, bool bAddBasePath) {
    string f;
    /*
	if (bAddBasePath) {
		f = fileName;
	}
	else {*/
    f = fileName;
    //}

    FILE* fp = fopen(f.c_str(), "wb");

    if (!fp) {
        assert(!"Unable to save data");
        return false;
    }
    unsigned int version = C_VARIANT_DB_FILE_VERSION;
    fwrite(&version, sizeof(unsigned int), 1, fp);

    dataList::iterator itor = m_data.begin();

    while (itor != m_data.end()) {
        Variant* pV = itor->second;

        if (!pV->Save(fp, itor->first)) {
            assert(!"Unable to save data");
            fclose(fp);
            return false;
        }

        itor++;
    }

    //add out done marker
    unsigned int doneMarker = Variant::TYPE_UNUSED;
    fwrite(&doneMarker, sizeof(unsigned int), 1, fp);

    fclose(fp);
    return true;
}

bool VariantDB::Load(const string& fileName, bool* pFileExistedOut, bool bAddBasePath) {
    FILE* fp = fopen(fileName.c_str(), "rb");
    if (!fp) {
        if (pFileExistedOut)
            *pFileExistedOut = false;
        return true; //not here, but that's not really an error
    }
    //get the version
    unsigned int version;
    if (pFileExistedOut)
        *pFileExistedOut = true;
    size_t bytesRead = fread(&version, 1, sizeof(unsigned int), fp);
    if (bytesRead == 0 || version != 1) {
        fclose(fp);
        assert(!"Unexpected version?!");
        return false;
    }

    string s;
    unsigned int varType;

    while (!feof(fp)) {
        fread(&varType, 1, sizeof(unsigned int), fp);
        if (varType == Variant::TYPE_UNUSED) {
            //out signal that we're done reading this part
            break;
        }
        //get the var name
        LoadFromFile(s, fp);
#ifdef _DEBUG
        if (GetVarIfExists(s) != NULL) {
            //LogMsg("VariantDB: variable %s already exists in database while loading from file %s. The previous value gets overwritten!", s.c_str(), fileName.c_str());
        }
#endif

        //get the actual data too
        switch (varType) {
            case Variant::TYPE_STRING: {
                string v;
                LoadFromFile(v, fp);
                GetVar(s)->Set(v);
                break;
            }

            case Variant::TYPE_UINT32: {
                unsigned int v;
                LoadFromFile(v, fp);
                GetVar(s)->Set(v);
                break;
            }

            case Variant::TYPE_INT32: {
                int v;
                LoadFromFile(v, fp);
                GetVar(s)->Set(v);
                break;
            }

            case Variant::TYPE_FLOAT: {
                float v;
                LoadFromFile(v, fp);
                GetVar(s)->Set(v);
                break;
            }

            case Variant::TYPE_VECTOR2: {
                CL_Vec2f v;
                LoadFromFile(v, fp);
                GetVar(s)->Set(v);
                break;
            }

            case Variant::TYPE_VECTOR3: {
                CL_Vec3f v;
                LoadFromFile(v, fp);
                GetVar(s)->Set(v);
                break;
            }

            case Variant::TYPE_RECT: {
                CL_Rectf v;
                LoadFromFile(v, fp);
                GetVar(s)->Set(v);
                break;
            }

            default:
                assert(!"%s - unknown var type");

                assert(!"Unknown var type");

                fclose(fp);
                return false;
        }
    }

    fclose(fp);

    return true;
}

void VariantDB::Print() {
    dataList::iterator itor = m_data.begin();
    printf("Listing VariantDB contents\n");
    printf("*********************\n");
    while (itor != m_data.end()) {
        Variant* pV = itor->second;
        if (itor->first == "deathAnim")
            pV->Set(0.0f);
        if (itor->first == "respawnAnim")
            pV->Set(0.0f);
        string s = itor->first + ": " + pV->Print();
        printf(s.c_str());
        printf(" (%d)\n", pV->GetType());
        itor++;
    }
    printf("*********************\n");
}

int VariantDB::DeleteVarsStartingWith(string deleteStr) {
    int deleted = 0;

    dataList::iterator itor = m_data.begin();

    while (itor != m_data.end()) {
        if (itor->first.compare(0, deleteStr.size(), deleteStr) == 0) {
            //match!
            delete (itor->second);
            dataList::iterator itorTemp = itor;
            itor++;

            m_data.erase(itorTemp);
            deleted++;
            continue;
        }
        itor++;
    }

    return deleted;
}

std::string VariantDB::DumpAsString() {
    string log = "*********************\r\nVARIANTDB DATA\r\n*********************\r\n";

    dataList::iterator itor = m_data.begin();

    while (itor != m_data.end()) {
        Variant* pV = itor->second;
        string s = itor->first + ": " + pV->Print();

        log += s + " ";
        itor++;
    }

    log += "\r\n";
    return log;
}
#include <sstream>
std::string VariantDB::DumpFunctionsAsString() {
    string log = "*********************\r\nVARIANTDB FUNCTIONS\r\n*********************\r\n";

    log += "\r\n";
    return log;
}

void VariantDB::ResetNext() {
    m_nextItor = m_data.begin();
}

Variant* VariantDB::GetNext(string& keyOut) {
    Variant* pReturn = NULL;

    if (m_nextItor == m_data.end()) {
        //all done
        ResetNext();
        return NULL;
    }

    keyOut = m_nextItor->first;
    pReturn = m_nextItor->second;
    m_nextItor++;
    return pReturn;
}
bool StringFromStartMatches(const std::string& line, const std::string textToMatch) {
    for (unsigned int i = 0; i < textToMatch.size(); i++) {
        if (i >= line.length())
            return false;
        if (line[i] != textToMatch[i])
            return false;
    }
    return true;
}
int VariantDB::AddVarPointersToVector(vector<pair<const string*, Variant*> >* varListOut, const string keyMustStartWithThisText /*=""*/) {
    int count = 0;
    dataList::iterator itor = m_data.begin();

    while (itor != m_data.end()) {
        //Variant *pV = itor->second;

        if (keyMustStartWithThisText.empty() || StringFromStartMatches(itor->first, keyMustStartWithThisText)) {
            varListOut->push_back(make_pair(&itor->first, itor->second));
            count++;
        }

        itor++;
    }

    return count;
}

void VariantDB::Clear() {
    m_data.clear();
    ResetNext();
}