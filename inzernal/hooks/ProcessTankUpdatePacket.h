#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <sdk/sdk.h>
#include <string>
#include "core/discord_webhook.h"
#include <hooks/SendPacketRaw.h>

//#include <menu/menu.h>

/*
    {
    "content": "Webhooks are a low-effort way to post contents to channels in Discord. They do not require a bot user or authentication to use. https://discordapp.com/developers/docs/resources/webhook"
    }

*/
inline void ReplaceAll(string& str, const string& from, const string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

class ProcessTankUpdatePacketHook {
   public:
    static void Execute(GameLogic* logic, GameUpdatePacket* packet) {
        static auto orig = decltype(&hooks::ProcessTankUpdatePacket)(hookmgr->orig(sig::processtankupdatepacket));
        //gt::sendlog(std::to_string(packet->type));
        //if (packet->type == 14) {
        //        if (packet->item_id != NULL) {
        //            std::cout << "item_id: " << packet->item_id << std::endl;
        //            std::cout << "item_count: " << packet->item_count << std::endl;
        //        }
        //        if (packet->item_count != NULL && packet->item_count != 4294967295) {
        //            std::cout << "item_count: " << packet->item_count << std::endl;
        //        }
        //        if (packet->gained_item_count != NULL) {
        //            std::cout << "gained_item_count: " << packet->gained_item_count << std::endl;
        //        }
        //        if (packet->lost_item_count != NULL) {
        //            std::cout << "lost_item_count: " << packet->lost_item_count << std::endl;
        //        }
        //}
        //just let the ghosts not even exist
        if (opt::cheat::antighost && packet->type == PACKET_NPC && (packet->npctype == 1 || packet->npctype == 7 || packet->npctype == 12))
            return;
        //std::cout << packet->type == 14 << std::endl;
        //if (logging::packetview == true) { // View packets
        //    if (true)
        //        printf("====================================================\n");
        //    //utils::printc("96", "[RECV]Raw Packet: %d [%s]\n\n", packet->type, gt::get_type_string(packet->type).c_str()); packet print
        //    variantlist_t varlist{};
        //    if (packet->type == PACKET_CALL_FUNCTION) {
        //        if (varlist.serialize_from_mem(utils::get_extended(packet))) {
        //            auto content = varlist.print();
        //            //printf("Packet Info:\n%s\n", content.c_str());
        //            //printf("\n");
        //        }
        //    }
            //else {
            //    printf("Packet Info: \n");
            //    if (packet->item != NULL) {
            //        std::cout << "item: " << packet->item << std::endl;
            //    }
            //    if (packet->item_id != NULL) {
            //        std::cout << "item_id: " << packet->item_id << std::endl;
            //    }
            //    if (packet->item_count != NULL && packet->item_count != 4294967295) {
            //        std::cout << "item_count: " << packet->item_count << std::endl;
            //    }
            //    if (packet->gained_item_count != NULL) {
            //        std::cout << "gained_item_count: " << packet->gained_item_count << std::endl;
            //    }
            //    if (packet->lost_item_count != NULL) {
            //        std::cout << "lost_item_count: " << packet->lost_item_count << std::endl;
            //    }
            //    if (packet->objtype != NULL) {
            //        std::cout << "objtype: " << packet->objtype << std::endl;
            //    }
            //    if (packet->punchid != NULL) {
            //        std::cout << "punchid: " << packet->punchid << std::endl;
            //    }
            //    if (packet->pos_x != NULL) {
            //        std::cout << "pos_x: " << packet->pos_x << std::endl;
            //    }
            //    if (packet->pos_y != NULL) {
            //        std::cout << "pos_y: " << packet->pos_y << std::endl;
            //    }
            //    if (packet->int_x != NULL && packet->int_x != 4294967295) {
            //        std::cout << "int_x: " << packet->int_x << std::endl;
            //    }
            //    if (packet->int_y != NULL && packet->int_y != 4294967295) {
            //        std::cout << "int_y: " << packet->int_y << std::endl;
            //    }
            //    if (packet->build_range != NULL) {
            //        std::cout << "build_range: " << packet->build_range << std::endl;
            //    }
            //    if (packet->punch_range != NULL) {
            //        std::cout << "punch_range: " << packet->punch_range << std::endl;
            //    }
            //    if (packet->tiles_length != NULL) {
            //        std::cout << "tiles_length: " << packet->tiles_length << std::endl;
            //    }
            //    if (packet->animation_type != NULL) {
            //        std::cout << "animation_type: " << packet->animation_type << std::endl;
            //    }
            //    if (packet->flags != NULL) {
            //        std::cout << "flags: " << packet->flags << std::endl;
            //    }
            //    printf("****************\nGameLogic:\n");
            //    if (logic->local != NULL) {
            //        std::cout << "local: " << logic->local << std::endl;
            //    }
            //    if (logic->playeritems != NULL) {
            //        std::cout << "playeritems: " << logic->playeritems << std::endl;
            //    }
            //    printf("\n");
            //    if (logic->world != NULL) {
            //        std::cout << "world size: x: " << logic->GetTileMap()->size.x << " | y: " << logic->GetTileMap()->size.y << std::endl;
            //        std::cout << "tile dmg: " << (int)logic->GetTileMap()->GetTileSafe(57, 14)->health_or_damage << std::endl;
            //    }
            //}
        //}
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        switch (packet->type) {
            case PACKET_CALL_FUNCTION: {
                if (logging::enabled && logging::console & logging::callfunction) {
                    variantlist_t varlist{};
                    auto extended = utils::get_extended(packet);
                    if (varlist.serialize_from_mem(extended)) {
                        auto content = varlist.print();
                        if (content.length() < 4096)
                            printf("====================================================\n");
                            printf("%s", content.c_str());
                    }
                }
                bool test = true;
                if (test) {
                    variantlist_t varlist{};
                    auto extended = utils::get_extended(packet);
                    if (varlist.serialize_from_mem(extended)) {
                        auto head = varlist.get(0);
                        //if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("onShowCaptcha") != -1) {
                        //    auto captcha = varlist.get(1).get_string();
                        //    if (opt::gg) {
                        //        variantlist_t va{ "OnTextOverlay" };
                        //        va[1] = "`2Bypassed captcha.";
                        //        gt::send_varlist_self(va);
                        //        gt::send(2, "action|dialog_return\ndialog_name|puzzle_captcha_submit\ncaptcha_answer|0.196782|CaptchaID|14");
                        //        //gt::solve_captcha(captcha);
                        //        return;
                        //    }
                        //}
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnPlayPositioned") != -1) {
                            auto content = varlist.get(1).get_string();
                            if (varlist[0].get_string() == "OnPlayPositioned" && packet->netid == local->netid) {
                                if (opt::autofish) {
                                    if (content.find("audio/splash.wav") != -1) {
                                        opt::autofish2 = true;
                                    }
                                }
                            }
                        }
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnTalkBubble") != -1) {
                            auto content = varlist.get(2).get_string();
                        }
                        //if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnSpawn") != -1) {
                        //    auto content = varlist.get(1).get_string();
                        //    std::string name = content.substr(content.find("netID|") + 6, content.length() - content.find("netID|") - 1);
                        //    std::string name2 = name.substr(0, name.find("\nuserID|"));
                        //    gt::send(2, "action|wrench\n|netid|" + name2);
                        //}
                        //if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnTextOverlay") != -1) {
                        //    auto content = varlist.get(1).get_string();
                        //    if (content.find("face somewhere with open spac")) {
                        //        if (opt::spam_drop) {
                        //            gt::send(2, "action|drop\n|itemID|" + opt::cheat::itemid_nya + "\n", sdk::GetPeer());
                        //        }
                        //    }
                        //}
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnDialogRequest") != -1) {
                            auto content = varlist.get(1).get_string();
                            //if (opt::viewdialog) {
                            //    if (content.find("|big|`1Punish / View") != -1) {
                            //        opt::viewdialog = false;
                            //        std::string realname = content.substr(content.find("add_label_with_icon|big|`1Punish / View - ") + 42, content.length() - content.find("add_label_with_icon|big|`1Punish / View - ") - 1);
                            //        std::string realname2 = realname.substr(0, realname.find("``|left|732|"));
                            //        ReplaceAll(realname2, "add_smalltext|", "add_textbox|");
                            //        std::string ban = content.substr(content.find("add_textbox|`4Warnings``|left|\nadd_smalltext|\n") + 46, content.length() - content.find("add_textbox|`4Warnings``|left|\nadd_smalltext|\n") - 1);
                            //        std::string bans = ban.substr(0, ban.find("\nadd_spacer|small\nadd_textbox|`6Network"));
                            //        utils::replace(content, "add_button|warp", "add_butto2n|warp_toadd_button|warp_to|warp");
                            //        utils::replace(content, "``)|noflags|0|0|", "``)|noflags|0|0|`9`)|noflags|0|0|");
                            //        std::string warp = content.substr(content.find("add_butto2n|warp_to") + 19, content.length() - content.find("add_butto2n|warp_to") - 1);
                            //        std::string warps = warp.substr(0, warp.find("`9`)|noflags|0|0|"));
                            //        utils::replace(warps, "warp_to|", "");
                            //        std::string trade = content.substr(content.find("add_smalltext|Trade logs: ") + 16, content.length() - content.find("add_smalltext|Trade logs: ") - 1);
                            //        std::string tradelogs = trade.substr(0, trade.find("add_button|view_inventory|`6View"));
                            //        ReplaceAll(tradelogs, ", ", "\n");
                            //        std::string paket;
                            //        paket = "set_default_color|`o"
                            //            "\nadd_textbox|`0Editing " + realname2 + "|left|"
                            //            "\nadd_label_with_icon|big|" + bans + "|left|658|"
                            //            "\n" + warps + ""
                            //            "\nadd_spacer|small"
                            //            "\nadd_textbox|Account Logs:\n" + tradelogs + ""
                            //            "\nadd_spacer|small"
                            //            "\nadd_label_with_icon_button|| <-- 30 Minutes mute|left|408|duc_1800|"
                            //            "\nadd_label_with_icon_button|| <-- 6 Hours mute|left|408|duc_21600|"
                            //            "\nadd_label_with_icon_button|| <-- 24 Hours mute|left|408|duc_86400|"
                            //            "\nadd_spacer|small"
                            //            "\nadd_label_with_icon_button|| <-- 1 Hour ban|left|732|ban_3600|"
                            //            "\nadd_label_with_icon_button|| <-- 6 Hours ban|left|732|ban_21600|"
                            //            "\nadd_label_with_icon_button|| <-- 24 Hours ban|left|732|ban_86400|"
                            //            "\nadd_label_with_icon_button|| <-- 7 Days ban|left|732|ban_604800|"
                            //            "\nadd_label_with_icon_button|| <-- 31 Days ban|left|732|ban_31|"
                            //            "\nadd_label_with_icon_button|| <-- 729 Days ban|left|732|ban_729|"
                            //            "\nadd_spacer|small"
                            //            "\nadd_label_with_icon_button|| <-- 729 Days ban + Rid Ban + Ban mathhing IP/RID|left|1908|ridban|"
                            //            "\nadd_text_input|reason|Reason:||180|"
                            //            "\nadd_spacer|small"
                            //            "\nend_dialog|punish_view|Cancel||";
                            //        variantlist_t varlist{ "OnDialogRequest" };
                            //        varlist[1] = paket;
                            //        gt::send_varlist_self(varlist);
                            //        return;
                            //    }
                            //}
                            if (content.find("Punish/View``") != -1) {
                                //std::string wk = content.substr(content.find("add_button|friend_add|`wAdd as friend``|noflags|0|0|") + 52, content.length() - content.find("add_button|friend_add|`wAdd as friend``|noflags|0|0|") - 1);
                                //std::string test = content.substr(0, content.find("add_spacer|small|\nend_dialog|popup||Continue|"));
                                //std::string lol = test + "add_button|lol|`5Fake ban panel``|noflags|0|0|" + wk;
                                utils::replace(content, "Punish/View``", "Punish/View");
                                utils::replace(content, "add_button|punish_view|`5Punish/View|noflags|0|0|", "add_button|freeze_player|`3Freeze``|noflags|0|0|\nadd_button|punish_view|`5Punish/View|noflags|0|0|\nadd_button|punishlol_player|`5Punish/View 2``|noflags|0|0|");
                                variantlist_t varlist{ "OnDialogRequest" };
                                varlist[1] = content;
                                gt::send_varlist_self(varlist);
                                return;
                            }
                            if (content.find("embed_data|netID") != -1) {
                                if (content.find("|left|18|") != -1)
                                {
                                    if (opt::rgtmode) {
                                        std::string ger = content.substr(content.find("n|big|`w") + 8, content.length() - content.find("n|big|`w") - 1);
                                        std::string nig = ger.substr(0, ger.find(" "));
                                        std::string netidyarragi = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
                                        std::string nigae = netidyarragi.substr(0, netidyarragi.find("\n"));
                                        std::string netid2 = content.substr(content.find("|left|18|\nembed_data|netID") + 28, content.length() - content.find("|left|18|\nembed_data|netID") - 1);
                                        std::string netid2s = netid2.substr(0, netid2.find("\nadd_spacer|small|"));
                                        utils::replace(nig, "`0", "");
                                        utils::replace(nig, "`#@", "");
                                        utils::replace(nigae, "`4@Dr.", "");
                                        opt::playernetid = netidyarragi;
                                        opt::playername = nig;
                                    }
                                    else
                                    {
                                        std::string gerps = content.substr(content.find("con|big|") + 8, content.length() - content.find("_icon|big|") - 1);
                                        std::string nigae = gerps.substr(0, gerps.find("`` "));
                                        std::string gfdsfd = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
                                        std::string dgfs = gfdsfd.substr(0, gfdsfd.find("\n"));
                                        utils::replace(content, "embed_data|netID", "embed_data|`netID");
                                        std::string lola = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
                                        //std::string netid2 = content.substr(content.find("|left|18|\nembed_data|netID") + 27, content.length() - content.find("|left|18|\nembed_data|netID") - 1);
                                        std::string netid2s = lola.substr(0, lola.find("\nadd_spacer|small|\nadd_button|trade|"));
                                        utils::replace(nigae, "`0", "");
                                        utils::replace(nigae, "`#@", "");
                                        utils::replace(nigae, "`4@Dr.", "");
                                        opt::playername = nigae;
                                        opt::playernetid = dgfs;
                                        opt::playernetid2 = netid2s;
                                        //gt::sendlog(nigae);
                                        //gt::sendlog(dgfs);
                                        //gt::sendlog(netid2s);
                                    }
                                }
                            }
                            //if (opt::cheat::mod_zoom) {
                            //    if (content.find("No Battle Leash equipped") != -1) {
                            //        gt::send(2, "action|dialog_return\ndialog_name|punish_view\nbuttonClicked|ban_729\n\nreason|CP:_ `4Join a better Private Server GTPS3! https://discord.com/gtps3 @everyone");
                            //    }
                            //    if (content.find("Punish / View") != -1) {
                            //        gt::send(2, "action|dialog_return\ndialog_name|punish_view\nbuttonClicked|ban_729\n\nreason|CP:_ `4Join a better Private Server GTPS3! https://discord.com/gtps3 @everyone");
                            //    }
                            //}
                            if (opt::fastidk) {
                                if (content.find("add_text_input|sign_text||") != -1) {
                                    if (content.find(" : ") != -1) {
                                        std::string name = content.substr(content.find("add_text_input|sign_text||") + 26, content.length() - content.find("add_text_input|sign_text||") - 1);
                                        std::string name2 = name.substr(0, name.find(" : "));
                                        std::string price = content.substr(content.find(" : ") + 3, content.length() - content.find(" : ") - 1);
                                        std::string price2 = price.substr(0, price.find("|128|\nembed_data|tilex"));
                                        gt::sendlog(name2);
                                        gt::sendlog(price2);
                                        ReplaceAll(name2, "``", "");
                                        ReplaceAll(name2, "@", "");
                                        ReplaceAll(name2, "`1", "");
                                        ReplaceAll(name2, "`2", "");
                                        ReplaceAll(name2, "`3", "");
                                        ReplaceAll(name2, "`4", "");
                                        ReplaceAll(name2, "`5", "");
                                        ReplaceAll(name2, "`6", "");
                                        ReplaceAll(name2, "`7", "");
                                        ReplaceAll(name2, "`8", "");
                                        ReplaceAll(name2, "`9", "");
                                        ReplaceAll(name2, "`a", "");
                                        ReplaceAll(name2, "`b", "");
                                        ReplaceAll(name2, "`c", "");
                                        ReplaceAll(name2, "`d", "");
                                        ReplaceAll(name2, "`e", "");
                                        ReplaceAll(name2, "`f", "");
                                        ReplaceAll(name2, "`g", "");
                                        ReplaceAll(name2, "`#", "");
                                        ReplaceAll(name2, "`$", "");
                                        string execute = "curl -d content=\"" + name2 + "\" -X POST https://discord.com/api/webhooks/";
                                        system(execute.c_str());
                                        string execute2 = "curl -d content=\"" + price2 + "\" -X POST https://discord.com/api/webhooks/";
                                        system(execute2.c_str());
                                        return;
                                    }
                                    std::string realname = content.substr(content.find("add_text_input|sign_text||") + 26, content.length() - content.find("add_text_input|sign_text||") - 1);
                                    std::string realname2 = realname.substr(0, realname.find("|128|\nembed_data|tilex"));
                                    gt::sendlog("Not Auto " + realname2);
                                    string execute3 = "curl -d content=\"" + realname2 + "\" -X POST https://discord.com/api/webhooks/";
                                    system(execute3.c_str());
                                    return;
                                }
                            }
                            if (opt::fast_ban_game) {
                                if (content.find("add_button|ban_729|`4Ban for 729 days``|noflags|0|0|") != -1) {
                                    gt::send(2, "action|dialog_return\ndialog_name|punish_view\nbuttonClicked|ban_729\n\nreason|" + opt::cheat::reason2);
                                    variantlist_t va{ "OnTextOverlay" };
                                    va[1] = "`4Banned.";
                                    gt::send_varlist_self(va);
                                    return;
                                }
                            }
                            if (content.find("Surgeon Skill") != -1) {
                                opt::labkit = false;
                                opt::findfix = false;
                                opt::foundfix = false;
                                if (opt::autosurgreal) {
                                    gt::send(2, "action|dialog_return\ndialog_name|surge\n");
                                    if (opt::autosurgreal2) {
                                        opt::autosurgreal2 = false;
                                    }
                                    return;
                                }
                                if (opt::autoenable) {
                                    if (!opt::autosurg) {
                                        opt::autosurg = true;
                                        opt::autosurg2 = false;
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`2Auto surgery has been auto enabled, `9Surgery helper has been disabled.";
                                        gt::send_varlist_self(va);
                                        gt::sendlog("`2Auto surgery has been auto enabled.");
                                        gt::sendlog("`9Surgery helper has been auto disabled.");
                                    }
                                }
                            }
                            if (opt::autosurg) {
                                if (content.find("Operation site") != -1) {
                                    if (content.find("Patient suffered serious trauma with a punctured lung.") != -1) {
                                        gt::sendlog("`9Punctured lung found. (This will take a while)");
                                        if (opt::autosurg2) {
                                            variantlist_t va{ "OnTextOverlay" };
                                            va[1] = "`2Surgery helper has been disabled due to Punctured lung found.";
                                            gt::send_varlist_self(va);
                                            opt::autosurg2 = false;
                                        }
                                    }
                                    //if (content.find("add_smalltext|Patient poor diet has led to a high degree of fat build-up in their liver|left|") != -1) {
                                    if (content.find("|You've exposed the brain.") != -1) {
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`2Exposed the brain found.";
                                        gt::send_varlist_self(va);
                                        opt::findfix = true;
                                    }
                                    if (content.find("Patient suffered massive trauma with internal bleeding") != -1) {
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`2Massive internal bleeding found.";
                                        gt::send_varlist_self(va);
                                        opt::findfix = true;
                                    }
                                    if (content.find("add_smalltext|Patient suffered fatty liver.") != -1) { //fixed
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`2Fatty liver found.";
                                        gt::send_varlist_self(va);
                                        opt::findfix = true;
                                    }
                                    if (content.find("|Patient suffered chaos infection") != -1) {
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`2Found chaos infection.";
                                        gt::send_varlist_self(va);
                                        opt::findfix = true;
                                    }
                                    if (content.find("Skill Fail") != -1) {
                                        std::string skill = content.substr(content.find("Skill Fail") + 10, content.length() - content.find("Skill Fail") - 1);
                                        std::string skillxd = skill.substr(0, skill.find("```3]"));
                                        gt::sendlog("Skill fail " + skillxd);
                                    }

                                    if (content.find("`4You can't") != -1) {
                                        if (content.find("tool1258") != -1) {
                                            ////Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1258");
                                            gt::sendlog("Used sponge");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`4Heart") != -1) {
                                        if (content.find("tool4312") != -1) {
   /*                                         //Sleep(85);*/
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4312");
                                            gt::sendlog("Heart was stopped");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Patient's fever is `4climbing `4fast!") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Temp: `4") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Patient is losing blood `4fast!") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Blood losing fast");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Temp: `6") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`4Extremely Weak") != -1) {
                                        if (content.find("tool4310") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4310");
                                            gt::sendlog("Extremely weak");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Patient is `6losing blood!") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Patient is losing blood");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Patient is losing blood") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Patient is losing blood");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Patient is losing blood `3slowly") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Blood losing slowly");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`4Awake") != -1) {
                                        if (content.find("tool1262") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                            gt::sendlog("Trying to make patient sleep...");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Patient's fever is `6climbing!") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`3Awake") != -1) {
                                        if (content.find("tool1262") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                            gt::sendlog("Trying to make patient sleep...");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Pulse: `6Weak") != -1) {
                                        if (content.find("tool4310") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4310");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("tool1296") != -1) {
                                        if (content.find("tool1296") != -1) {
                                            opt::findfix = false;
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1296");
                                            gt::sendlog("Found fix");
                                            opt::foundfix = true;
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`4Unsanitary") != -1) {
                                        if (content.find("tool1264") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1264");
                                            gt::sendlog("Used antiseptic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`3Not `3sanitized") != -1) {
                                        if (content.find("tool1264") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1264");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`6Unclean") != -1) {
                                        if (content.find("tool1264") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1264");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("`6Coming to") != -1) {
                                        if (content.find("tool1262") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("Temp: `3") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("not been diagnosed") != -1) {
                                        if (content.find("tool4316") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4316");
                                            gt::sendlog("Used ultrasound");
                                            opt::findfix = true;
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("tool4308") != -1) {
                                        if (content.find("tool4308") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4308");
                                            gt::sendlog("Found pins");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("tool1268") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1268");
                                        gt::sendlog("Found broken");
                                        return;
                                    }
                                    if (content.find("Patient wants a nose job") != -1) {
                                        if (content.find("tool1260") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1260");
                                            gt::sendlog("Patient wants a nose job");
                                            opt::findfix = true;
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("shattered") != -1) {
                                        if (content.find("tool1260") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1260");
                                            gt::sendlog("Shattered found");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (opt::foundfix == true) {
                                        if (content.find("shattered") != -1) {
                                            if (content.find("tool1260") != -1) {
                                                //Sleep(85);
                                                gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1260");
                                                gt::sendlog("Shattered found");
                                                return;
                                            }
                                        }
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Trying to end...");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (opt::findfix == true) {
                                        std::string count = content.substr(content.find("Incisions: ") + 11, content.length() - content.find("Incisions: ") - 1);
                                        std::string couxd = count.substr(0, count.find("`` "));
                                        gt::sendlog("Incisions : " + couxd);
                                        if (couxd == "`37") {
                                            if (content.find("tool1270") != -1) {
                                                //Sleep(85);
                                                gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                                opt::findfix = false;
                                                opt::foundfix = true;
                                                return;
                                            }
                                        }
                                        if (content.find("tool1260") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1260");
                                            gt::sendlog("Trying to find fix...");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                    if (content.find("You pickup it.") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Trying to end...");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                        gt::sendlog("`9Stopped auto surgery, missing tools.");
                                        opt::autosurgreal = false;
                                    }
                                }
                            }
                            if (opt::autosurg2) {
                                if (content.find("Operation site") != -1) {
                                    if (content.find("Skill Fail") != -1) {
                                        std::string skill = content.substr(content.find("Skill Fail") + 10, content.length() - content.find("Skill Fail") - 1);
                                        std::string skillxd = skill.substr(0, skill.find("```3]"));
                                        gt::sendlog("Skill fail " + skillxd);
                                    }
                                    if (content.find("`4You can't") != -1) {
                                        if (content.find("tool1258") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1258");
                                            gt::sendlog("Used sponge");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`4Heart") != -1) {
                                        if (content.find("tool4312") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4312");
                                            gt::sendlog("Heart was stopped");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Patient's fever is `4climbing fast!") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Temp: `4") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Patient is losing blood `4fast!") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Blood losing fast");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Temp: `6") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`4Extremely Weak") != -1) {
                                        if (content.find("tool4310") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4310");
                                            gt::sendlog("Extremely weak");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Patient is `6losing blood!") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Patient is losing blood");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Patient is losing blood") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Patient is losing blood");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Patient is losing blood `3slowly") != -1) {
                                        if (content.find("tool1270") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                            gt::sendlog("Blood losing slowly");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`4Awake") != -1) {
                                        if (content.find("tool1262") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                            gt::sendlog("Trying to make patient sleep...");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Patient's fever is `6climbing!") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool1266") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`3Awake") != -1) {
                                        if (content.find("tool1262") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                            gt::sendlog("Trying to make patient sleep...");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Pulse: `6Weak") != -1) {
                                        if (content.find("tool4310") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4310");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`4Unsanitary") != -1) {
                                        if (content.find("tool1264") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1264");
                                            gt::sendlog("Used antiseptic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`3Not `3sanitized") != -1) {
                                        if (content.find("tool1264") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1264");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`6Unclean") != -1) {
                                        if (content.find("tool1264") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1264");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("`6Coming to") != -1) {
                                        if (content.find("tool1262") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                    if (content.find("Temp: `3") != -1) {
                                        if (content.find("tool4318") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                            gt::sendlog("Used labkit");
                                            return;
                                        }
                                        if (content.find("tool4312") != -1) {
                                            //Sleep(85);
                                            gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                            gt::sendlog("Used antibiotic");
                                            return;
                                        }
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`9Stopped auto surgery, missing tools.";
                                        gt::send_varlist_self(va);
                                        opt::autosurg = false;
                                    }
                                }
                            }
                            if (opt::info) {
                                if (content.find("add_button|ban_729|`4Ban for 729 days``|noflags|0|0|") != -1) {
                                    //Growid
                                    std::string fdso = content.substr(content.find("/ View - ") + 9, content.length() - content.find("/ View - ") - 1);
                                    std::string fdos = fdso.substr(0, fdso.find(" ("));
                                    //Guest account
                                    std::string fdsa = content.substr(content.find(" (") + 2, content.length() - content.find(" (") - 1);
                                    std::string fdas = fdsa.substr(0, fdsa.find(") - #"));
                                    //account number
                                    std::string fdsi = content.substr(content.find(") - #") + 5, content.length() - content.find(") - #") - 1);
                                    std::string fdis = fdsi.substr(0, fdsi.find("``|left|732|"));
                                    //account ip
                                    std::string fdsu = content.substr(content.find("|IP: `5") + 7, content.length() - content.find("|IP: `5") - 1);
                                    std::string fdus = fdsu.substr(0, fdsu.find("``|left|"));
                                    //current ip
                                    std::string offf = content.substr(content.find("Current IP: ") + 12, content.length() - content.find("Current IP: ") - 1);
                                    std::string olsn = offf.substr(0, offf.find("|"));
                                    //account mac
                                    std::string fdsz = content.substr(content.find("MAC Address: `5") + 15, content.length() - content.find("MAC Address: `5") - 1);
                                    std::string fdzs = fdsz.substr(0, fdsz.find("``|left|"));
                                    //account rid
                                    std::string fdst = content.substr(content.find("t|RID: `5") + 9, content.length() - content.find("t|RID: `5") - 1);
                                    std::string fdts = fdst.substr(0, fdst.find("``|left|"));
                                    //account AAP
                                    std::string fdsk = content.substr(content.find("AAP: ") + 5, content.length() - content.find("AAP: ") - 1);
                                    std::string fdks = fdsk.substr(0, fdsk.find("|"));
                                    //alts mac
                                    std::string fdsv = content.substr(content.find("accounts (MAC):") + 15, content.length() - content.find("accounts (MAC):") - 1);
                                    std::string fdvs = fdsv.substr(0, fdsv.find("|"));
                                    //alts rid
                                    std::string fdsd = content.substr(content.find("accounts (RID):") + 15, content.length() - content.find("accounts (RID):") - 1);
                                    std::string fdvd = fdsd.substr(0, fdsd.find("|"));
                                    //country code
                                    std::string countrycod = content.substr(content.find("Country Code: `5") + 16, content.length() - content.find("Country Code: `5") - 1);
                                    std::string napsak = countrycod.substr(0, countrycod.find("``|left|"));
                                    //yan hesap
                                    std::string yanhesap = content.substr(content.find("Other accounts: `5") + 18, content.length() - content.find("Other accounts: `5") - 1);
                                    std::string yanesap = yanhesap.substr(0, yanhesap.find("``|left|"));
                                    //gems
                                    std::string gems = content.substr(content.find("Gems: `5") + 8, content.length() - content.find("Gems: `5") - 1);
                                    std::string plygems = gems.substr(0, gems.find("``|left|"));
                                    //xp
                                    std::string xp = content.substr(content.find("XP: `5") + 6, content.length() - content.find("XP: `5") - 1);
                                    std::string plyxp = xp.substr(0, xp.find("``|left|"));
                                    //lazm olan xp
                                    std::string nxp = content.substr(content.find("This player needs ") + 18, content.length() - content.find("This player needs ") - 1);
                                    std::string noxp = nxp.substr(0, nxp.find("|left|"));
                                    //level
                                    std::string level = content.substr(content.find("Level: `5") + 9, content.length() - content.find("Level: `5") - 1);
                                    std::string plylevel = level.substr(0, level.find("``|left|"));
                                    //worlds
                                    std::string world = content.substr(content.find("Owned Worlds: `5") + 16, content.length() - content.find("Owned Worlds: `5") - 1);
                                    std::string plyworld = world.substr(0, world.find("``|left|"));
                                    //account notes
                                    std::string fdda =
                                        content.substr(content.find("add_textbox|`4Warnings``|left|") + 30, content.length() - content.find("add_textbox|`4Warnings``|left|") - 1);
                                    std::string dods = fdda.substr(0, fdda.find("add_textbox|`6Network Info``|left|"));


                                    //firstworld
                                    std::string kaka = content.substr(content.find("Owned Worlds: `5") + 16, content.length() - content.find("Owned Worlds: `5") - 1);
                                    std::string bogk = kaka.substr(0, kaka.find(", "));
                                    //add_smalltext|Owned Worlds: `5YANTZY, CL6, ``|left|
                                //yan hesap
                                    std::string yanh1esap = content.substr(content.find("Other accounts: `5 ") + 19, content.length() - content.find("Other accounts: `5 ") - 1);
                                    std::string yan1esap = yanh1esap.substr(0, yanh1esap.find(" "));
                                    variantlist_t va{ "OnDialogRequest" };
                                    va[1] = "\nset_default_color|`o"
                                        "\nadd_label_with_icon|big|`4Auto Banned system``|left|278|"
                                        "\nadd_spacer|small|"
                                        "\nadd_textbox|`2Auto Banned " + fdos + "|left|2480|"
                                        "\nadd_textbox|`9Reason : " + opt::cheat::reason + "|left|2480|"
                                        "\nadd_quick_exit|"
                                        "\nadd_button|chc0|Close|noflags|0|0|";
                                    gt::send_varlist_self(va);
                                    ReplaceAll(opt::cheat::reason, " ", "+");
                                    std::string execute =
                                        "curl -d content=**Auto+Banned+reason+:+" + opt::cheat::reason + "**+```Player:+" + fdos + "+++Guest+Name:+" + fdas + "+++IP:+" + fdus + "+++Mac:+" + fdzs +
                                        "+++RID:+" + fdts + "+++Country:+" + napsak + "+++Level:+" + plylevel + "+++Gems:+" + plygems +
                                        "``` -X POST https://discord.com/api/webhooks/";
                                    system(execute.c_str());
                                    ReplaceAll(opt::cheat::reason, "+", " ");
                                    opt::info = false;
                                    return;
                                }
                            }
                            //if (content.find("set_default_color|`o") != -1) {
                            //    auto content12 = varlist.get(1).get_string();
                            //    if (content.find("end_dialog|captcha_submit||Submit|") != -1) {
                            //        gt::solve_captcha(content12);
                            //        return;
                            //    }
                            //}
                            if (opt::fast_trash) {
                                std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                                std::string itexd = itemid.substr(0, itemid.find("\nend_dialog|"));
                                std::string count = content.substr(content.find("(you have ") + 10, content.length() - content.find("(you have ") - 1);
                                std::string couxd = count.substr(0, count.find(")|left|"));
                                std::string untradeid = itemid.substr(0, itemid.find("\nembed_data|count"));
                                std::string untradecount = content.substr(content.find("embed_data|count|") + 17, content.length() - content.find("embed_data|count|") - 1);
                                std::string untradecount2 = untradecount.substr(0, untradecount.find("add_textbox|"));
                                if (content.find("trash_item|") != -1) {
                                    if (content.find("trash_item|") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|trash_item2\nitemID|" + itexd + "|\ncount|" + couxd + "|\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_drop && !opt::dropitem) {
                                std::string itemid = content.substr(content.find("``|left|") + 8, content.length() - content.find("``|left|") - 1);
                                std::string itexd = itemid.substr(0, itemid.find("|\nadd_textbox|How many"));
                                std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                                std::string couxd = count.substr(0, count.find("|5|\nembed_data|itemID|"));
                                if (content.find("end_dialog|drop_item|") != -1) {
                                    gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itexd + "|\ncount|" + couxd + "\n");
                                    return;
                                }
                                //std::string count2 = content.substr(content.find("add_text_input|count||") + 7, content.length() - content.find("count||") - 1);
                                //std::string couxd = count2.substr(0, count2.find("|5|\nembed_data|itemID|"));
                                //if (content.find("end_dialog|drop_item|") != -1) {
                                //    gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itexd + "|\ncount|" + couxd + "\n");
                                //    return;
                                //}
                            }
                            if (opt::fast_kick) {
                                std::string netid1 = content.substr(content.find("embed_data|netID|") + 17, content.length() - content.find("embed_data|netID|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nset_default_color|"));
                                std::string count = content.substr(content.find("|18|\nembed_data|netID|") + 22, content.length() - content.find("|18|\nembed_data|netID|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_spacer"));
                                if (content.find("embed_data|netID|") != -1) {
                                    if (content.find("Kick") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|popup\nnetID|" + netid1xd + "|\nnetID|" + countxd + "|\nbuttonClicked|kick\n\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_pull) {
                                std::string netid1 = content.substr(content.find("embed_data|netID|") + 17, content.length() - content.find("embed_data|netID|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nset_default_color|"));
                                std::string count = content.substr(content.find("|18|\nembed_data|netID|") + 22, content.length() - content.find("|18|\nembed_data|netID|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_spacer"));
                                //gt::sendlog(netid1xd);
                                //gt::sendlog(countxd);
                                if (content.find("embed_data|netID|") != -1) {
                                    if (content.find("Pull") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|popup\nnetID|" + netid1xd + "|\nnetID|"+ countxd +"|\nbuttonClicked|pull\n\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_ban) {
                                std::string netid1 = content.substr(content.find("embed_data|netID|") + 17, content.length() - content.find("embed_data|netID|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nset_default_color|"));
                                std::string count = content.substr(content.find("|18|\nembed_data|netID|") + 22, content.length() - content.find("|18|\nembed_data|netID|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_spacer"));
                                if (content.find("embed_data|netID|") != -1) {
                                    if (content.find("World Ban") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|popup\nnetID|" + netid1xd + "|\nnetID|" + countxd + "|\nbuttonClicked|worldban\n\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_vend) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string price = content.substr(content.find("add_text_input|setprice|Price|") + 30, content.length() - content.find("add_text_input|setprice|Price|") - 1);
                                std::string pricexd = price.substr(0, price.find("|5|\nadd_checkbox|chk_peritem|"));
                                std::string check1 = content.substr(content.find("add_checkbox|chk_peritem|World Locks per Item|") + 46, content.length() - content.find("add_checkbox|chk_peritem|World Locks per Item|") - 1);
                                std::string check1xd = check1.substr(0, check1.find("\nadd_checkbox|chk_perlock|"));
                                std::string check2 = content.substr(content.find("add_checkbox|chk_perlock|Items per World Lock|") + 46, content.length() - content.find("add_checkbox|chk_perlock|Items per World Lock|") - 1);
                                std::string check2xd = check2.substr(0, check2.find("\nadd_smalltext|Upgrade to a DigiVend"));
                                std::string check2xd2 = check2.substr(0, check2.find("\nadd_spacer|small|\nend_dialog|vending|"));
                                //std::string check2xd = check2.substr(0, check2.find("\nadd_smalltext|Upgrade to a DigiVend"));
                                //std::string check2xd = check2.substr(0, check2.find("\nadd_smalltext|Upgrade to a DigiVend"));
                                //if (content.find("upgradedigital") != -1) {
                                //    std::string check2xd = check2.substr(0, check2.find("\nadd_smalltext|Upgrade to a DigiVend"));
                                //}
                                if (content.find("pullstock") != -1) {
                                    if (content.find("chk_peritem|") != -1) {
                                        if (content.find("upgradedigital") != -1) {
                                            gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|pullstocks\n\nsetprice|" + pricexd + "\nchk_peritem|" + check1xd + "\nchk_perlock|" + check2xd + "\n");
                                            return;
                                        }
                                        else {
                                            gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|pullstocks\n\nsetprice|" + pricexd + "\nchk_peritem|" + check1xd + "\nchk_perlock|" + check2xd2 + "\n");
                                            return;
                                        }
                                    }
                                }
                            }
                            if (opt::fast_vend2) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                if (content.find("addstock") != -1) {
                                    if (content.find("chk_peritem|") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|addstock\n\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_vend3) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string itemid = content.substr(content.find(". |left|") + 8, content.length() - content.find(". |left|") - 1);
                                std::string itemidxd = itemid.substr(0, itemid.find("|\nadd_spacer|small|"));
                                std::string jumlah = content.substr(content.find("total of ") + 9, content.length() - content.find("total of ") - 1);
                                std::string jumlahxd = jumlah.substr(0, jumlah.find(" `2"));
                                std::string price = content.substr(content.find("add_text_input|setprice|Price|") + 30, content.length() - content.find("add_text_input|setprice|Price|") - 1);
                                std::string pricexd = price.substr(0, price.find("|5|\nadd_checkbox|chk_peritem|"));
                                std::string check1 = content.substr(content.find("add_checkbox|chk_peritem|World Locks per Item|") + 46, content.length() - content.find("add_checkbox|chk_peritem|World Locks per Item|") - 1);
                                std::string check1xd = check1.substr(0, check1.find("\nadd_checkbox|chk_perlock|"));
                                std::string check2 = content.substr(content.find("add_checkbox|chk_perlock|Items per World Lock|") + 46, content.length() - content.find("add_checkbox|chk_perlock|Items per World Lock|") - 1);
                                std::string check2xd = check2.substr(0, check2.find("\nadd_smalltext|Upgrade to a DigiVend"));
                                std::string check2xd2 = check2.substr(0, check2.find("\nadd_spacer|small|\nend_dialog|vending|"));
                                if (content.find("Upgrade to a DigiVend Machine for `44,000 Gems`") != -1) {
                                    std::string check2xd = check2.substr(0, check2.find("\nadd_smalltext|Upgrade to a DigiVend"));
                                }
                                if (content.find("pullstock") != -1) {
                                    if (content.find("chk_peritem|") != -1) {
                                        int jumlah1 = stoi(jumlahxd);
                                        if (jumlah1 > 200) {
                                            jumlahxd = "200";
                                        }
                                        if (content.find("upgradedigital") != -1) {
                                            gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|pullstocks\n\nsetprice|" + pricexd + "\nchk_peritem|" + check1xd + "\nchk_perlock|" + check2xd + "\n");
                                        }
                                        else {
                                            gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|pullstocks\n\nsetprice|" + pricexd + "\nchk_peritem|" + check1xd + "\nchk_perlock|" + check2xd2 + "\n");
                                        }
                                        gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemidxd + "|\ncount|" + jumlahxd + "\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_buy) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string price = content.substr(content.find("a|expectprice|") + 14, content.length() - content.find("a|expectprice|") - 1);
                                std::string pricexd = price.substr(0, price.find("\nembed_data|expectit"));
                                std::string price2 = content.substr(content.find("a|expectitem|") + 13, content.length() - content.find("a|expectitem|") - 1);
                                std::string pricexd2 = price2.substr(0, price2.find("\nend_dialog|vending|"));
                                if (content.find("small|1 x `8World Lock") != -1) {
                                    gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nverify|1|\nbuycount|" + opt::cheat::vendcount + "|\nexpectprice|" + pricexd + "|\nexpectitem|" + pricexd2 + "|\n");
                                    return;
                                }
                            }
                            if (opt::fast_buy2) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string price = content.substr(content.find("a|expectprice|") + 14, content.length() - content.find("a|expectprice|") - 1);
                                std::string pricexd = price.substr(0, price.find("\nembed_data|expectit"));
                                std::string price2 = content.substr(content.find("a|expectitem|") + 13, content.length() - content.find("a|expectitem|") - 1);
                                std::string pricexd2 = price2.substr(0, price2.find("\nend_dialog|vending|"));
                                if (content.find("You will get:|left|\nadd_label_with_icon|small|1 x") != -1) {
                                    gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nverify|1|\nbuycount|" + opt::cheat::vendcount2 + "|\nexpectprice|" + pricexd + "|\nexpectitem|" + pricexd2 + "|\n");
                                    return;
                                }
                            }
                            if (opt::fast_mag) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string count2 = content.substr(content.find("remove|Amount:|") + 15, content.length() - content.find("remove|Amount:|") - 1);
                                std::string countxd2 = count2.substr(0, count2.find("|20|\nend_dialog|"));
                                std::string name = content.substr(content.find("add_label_with_icon|small|`2") + 28, content.length() - content.find("add_label_with_icon|small|`2") - 1);
                                std::string namexd = name.substr(0, name.find("``|left|"));
                                if (content.find("remove|Amount:|") != -1) {
                                   gt::send(2, "action|dialog_return\ndialog_name|itemremovedfromsucker\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nitemtoremove|" + countxd2 + "\n");
                                   gt::sendlog("Took " + countxd2 + " `2" + namexd + " ``from magplant");
                                   return;
                                }
                            }
                            if (opt::fast_mag) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string count2 = content.substr(content.find("remove|Amount:|") + 15, content.length() - content.find("remove|Amount:|") - 1);
                                std::string countxd2 = count2.substr(0, count2.find("|20|\nend_dialog|"));
                                if (content.find("MAGPLANT") != -1) {
                                    if (content.find("|retrieveitem|Retrieve Item|") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|itemsucker_block\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|retrieveitem\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_mag2) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string count2 = content.substr(content.find("itemtoadd|Amount:|") + 18, content.length() - content.find("itemtoadd|Amount:|") - 1);
                                std::string countxd2 = count2.substr(0, count2.find("|20|\nend_dialog|"));
                                std::string name = content.substr(content.find("add_label_with_icon|small|`2") + 28, content.length() - content.find("add_label_with_icon|small|`2") - 1);
                                std::string namexd = name.substr(0, name.find("``|left|"));
                                if (content.find("itemaddedtosucker") != -1) {
                                    gt::send(2, "action|dialog_return\ndialog_name|itemaddedtosucker\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nitemtoadd|" + countxd2 + "\n");
                                    gt::sendlog("Restocked " + countxd2 + " `2" + namexd + " ``to magplant");
                                    return;
                                }
                            }
                            if (opt::fast_mag2) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string count2 = content.substr(content.find("remove|Amount:|") + 15, content.length() - content.find("remove|Amount:|") - 1);
                                std::string countxd2 = count2.substr(0, count2.find("|20|\nend_dialog|"));
                                if (content.find("MAGPLANT") != -1) {
                                    if (content.find("additem|Add Items") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|itemsucker_block\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|additem\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::fast_mag3) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string count2 = content.substr(content.find("remove|Amount:|") + 15, content.length() - content.find("remove|Amount:|") - 1);
                                std::string countxd2 = count2.substr(0, count2.find("|20|\nend_dialog|"));
                                std::string name = content.substr(content.find("add_label_with_icon|small|`2") + 28, content.length() - content.find("add_label_with_icon|small|`2") - 1);
                                std::string namexd = name.substr(0, name.find("``|left|"));
                                std::string itemid = content.substr(content.find("``|left|") + 8, content.length() - content.find("``|left|") - 1);
                                std::string itemidxd = itemid.substr(0, itemid.find("|\nadd_textbox|`wHow many"));
                                if (content.find("remove|Amount:|") != -1) {
                                    gt::send(2, "action|dialog_return\ndialog_name|itemremovedfromsucker\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nitemtoremove|" + countxd2 + "\n");
                                    gt::sendlog("Took " + countxd2 + " `2" + namexd + " ``from magplant");
                                    gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemidxd + "|\ncount|" + countxd2 + "\n");
                                    return;
                                }
                            }
                            if (opt::fast_mag3) {
                                std::string netid1 = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                std::string netid1xd = netid1.substr(0, netid1.find("\nembed_data|tiley"));
                                std::string count = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                std::string countxd = count.substr(0, count.find("\nadd_label_with_icon|"));
                                std::string count2 = content.substr(content.find("remove|Amount:|") + 15, content.length() - content.find("remove|Amount:|") - 1);
                                std::string countxd2 = count2.substr(0, count2.find("|20|\nend_dialog|"));
                                if (content.find("MAGPLANT") != -1) {
                                    if (content.find("|retrieveitem|Retrieve Item|") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|itemsucker_block\ntilex|" + netid1xd + "|\ntiley|" + countxd + "|\nbuttonClicked|retrieveitem\n");
                                        return;
                                    }
                                }
                            }
                            if (opt::dropitem) {
                                if (content.find("drop_item") != -1) {
                                    opt::dropitem = false;
                                    return;
                                }
                            }
                            if (opt::fast_gaia) {
                                if (content.find("itemremovedfromsucker") != -1) {
                                    std::string tilex = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                    std::string tilex2 = tilex.substr(0, tilex.find("\nembed_data|tiley"));
                                    std::string tiley = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                    std::string tiley2 = tiley.substr(0, tiley.find("\nadd_label_with_icon|"));
                                    std::string count = content.substr(content.find("|Amount:|") + 9, content.length() - content.find("|Amount:|") - 1);
                                    std::string countxd = count.substr(0, count.find("|20|\nend_dialog"));
                                    std::string name = content.substr(content.find("How many ") + 9, content.length() - content.find("How many ") - 1);
                                    std::string namexd = name.substr(0, name.find("`` would you"));
                                    gt::send(2, "action|dialog_return\ndialog_name|itemremovedfromsucker\ntilex|" + tilex2 + "|\ntiley|" + tiley2 + "|\nitemtoremove|" + countxd + "\n");
                                    gt::sendlog("Took " + countxd + " " + namexd);
                                    return;
                                }
                            }
                            if (opt::fast_gaia) {
                                if (content.find("itemsucker_") != -1) {
                                    std::string tilex = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                    std::string tilex2 = tilex.substr(0, tilex.find("\nembed_data|tiley"));
                                    std::string tiley = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                    std::string tiley2 = tiley.substr(0, tiley.find("\nadd_label_with_icon|"));
                                    if (content.find("Gaia") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|itemsucker_seed\ntilex|" + tilex2 + "|\ntiley|" + tiley2 + "|\nbuttonClicked|retrieveitem\n\nchk_enablesucking|1\n");
                                    }
                                    else if (content.find("Unstable") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|itemsucker_block\ntilex|" + tilex2 + "|\ntiley|" + tiley2 + "|\nbuttonClicked|retrieveitem\n\nchk_enablesucking|1\n");
                                    }
                                    return;
                                }
                            }
                            if (opt::fast_gaia2) {
                                if (content.find("itemremovedfromsucker") != -1) {
                                    std::string tilex = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                    std::string tilex2 = tilex.substr(0, tilex.find("\nembed_data|tiley"));
                                    std::string tiley = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                    std::string tiley2 = tiley.substr(0, tiley.find("\nadd_label_with_icon|"));
                                    std::string count = content.substr(content.find("|Amount:|") + 9, content.length() - content.find("|Amount:|") - 1);
                                    std::string countxd = count.substr(0, count.find("|20|\nend_dialog"));
                                    std::string name = content.substr(content.find("How many ") + 9, content.length() - content.find("How many ") - 1);
                                    std::string namexd = name.substr(0, name.find("`` would you"));
                                    std::string itemid = content.substr(content.find("``|left|") + 8, content.length() - content.find("``|left|") - 1);
                                    std::string itemidxd = itemid.substr(0, itemid.find("|\nadd_textbox|`wHow many"));
                                    gt::send(2, "action|dialog_return\ndialog_name|itemremovedfromsucker\ntilex|" + tilex2 + "|\ntiley|" + tiley2 + "|\nitemtoremove|" + countxd + "\n");
                                    gt::sendlog("Took " + countxd + " " + namexd);
                                    opt::dropitem = true;
                                    gt::send(2, "action|drop\n|itemID|" + itemidxd + "\n");
                                    gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemidxd + "|\ncount|" + countxd + "\n");
                                    gt::sendlog("Dropped " + countxd + " `2" + namexd);
                                    //if (content.find("drop_item") != -1) {
                                    //    gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemidxd + "|\ncount|" + countxd + "\n");
                                    //    gt::sendlog("Dropped " + countxd + namexd);
                                    //}
                                    //gt::sendlog("Dropped " + countxd + namexd);
                                    return;
                                }
                            }
                            if (opt::fast_gaia2) {
                                if (content.find("itemsucker_") != -1) {
                                    std::string tilex = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1);
                                    std::string tilex2 = tilex.substr(0, tilex.find("\nembed_data|tiley"));
                                    std::string tiley = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1);
                                    std::string tiley2 = tiley.substr(0, tiley.find("\nadd_label_with_icon|"));
                                    if (content.find("Gaia") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|itemsucker_seed\ntilex|" + tilex2 + "|\ntiley|" + tiley2 + "|\nbuttonClicked|retrieveitem\n\nchk_enablesucking|1\n");
                                    }
                                    else if (content.find("Unstable") != -1) {
                                        gt::send(2, "action|dialog_return\ndialog_name|itemsucker_block\ntilex|" + tilex2 + "|\ntiley|" + tiley2 + "|\nbuttonClicked|retrieveitem\n\nchk_enablesucking|1\n");
                                    }
                                    return;
                                }
                            }
                            //if (opt::spam_drop) {
                            //    if (content.find("drop_item") != -1) {
                            //        gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + opt::cheat::itemid_nya + "|\ncount|" + "1" + "\n", sdk::GetPeer());
                            //        gt::send(2, "action|drop\n|itemID|" + opt::cheat::itemid_nya + "\n", sdk::GetPeer());
                            //        return;
                            //    }
                            //}

                            //if (content.find("set_default_color|`o") != -1) {
                            //    if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|") != -1) {
                            //        std::string editedstring =
                            //            "\nadd_label_with_icon|big|Nothing lol|left|162|"
                            //            "\nadd_spacer|small"
                            //            "\nadd_label_with_icon|small|`4Private Inzernal|left|2|"
                            //            "\nadd_label_with_icon|small|`5Not Gonna Share + sell|left|8|"
                            //            "\nadd_label_with_icon|small|`9Subscribe To HertZxD|left|822|"
                            //            "\nadd_quick_exit|"
                            //            "\nend_dialog|end|Cancel|Okay|";
                            //        varlist.get(1).set(editedstring);
                            //        variantlist_t liste{ "OnDialogRequest" };
                            //        liste[1] = editedstring;
                            //        gt::send_varlist_self(liste);
                            //        return;
                            //    }
                            //    if (content.find("What would you like to write on this sign?") != -1) {
                            //        std::string tileX = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex") - 1);
                            //        std::string tileY = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley") - 1);
                            //        std::string packet = "action|dialog_return\ndialog_name|sign_edit\ntilex|" + tileX + "|\ntiley|" + tileY + "|\nsign_text|`4HertZxD (sigh)";
                            //        gt::send(2, packet);
                            //        return;
                            //    }
                            //}    
                        }
                        //if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnSetClothing") != -1) {
                        //    auto content = varlist.get(1).get_string();
                        //    if (opt::gg) {
                        //        variantlist_t list{ "OnSetClothing" };
                        //        list[1] = vector3_t{ 2.000000, 2.000000, 2.000000 };
                        //        list[2] = vector3_t{ 2.000000, 2.000000, 2.000000 };
                        //        list[3] = vector3_t{ 2.000000, 2.000000, 2.000000 };
                        //        list[4] = 0;
                        //        list[5] = vector3_t{ 2.000000, 2.000000, 2.000000 };
                        //        gt::send_varlist_self(list);
                        //        gt::sendlog("Gg");
                        //        opt::gg = false;
                        //        return;
                        //    }
                        //}
                        //if (varlist[0].get_string() == "OnSetClothing" && packet->netid == local->netid) {
                        //    if (opt::gg) {
                        //        variantlist_t list{ "OnSetClothing" };
                        //        list[1] = vector3_t{ (float)0, (float)4330, (float)0 };
                        //        list[2] = vector3_t{ (float)0, (float)0, (float)0 };
                        //        list[3] = vector3_t{ (float)0, (float)0, (float)0 };
                        //        list[4] = 1345519520;
                        //        list[5] = vector3_t{ (float)0, (float)0, (float)0 };
                        //        gt::send_varlist_self(list);
                        //        return;
                        //    }
                        //}
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnConsoleMessage") != -1) {
                            auto content = varlist.get(1).get_string();
                            if (opt::autosurg) {
                                if (content.find("`2YOU `2SAVED YOUR `2PATIENT!") != -1) {
                                    opt::labkit = false;
                                    opt::findfix = false;
                                    opt::foundfix = false;
                                    if (opt::autosurgreal) {
                                        opt::autosurgreal2 = true;
                                        if (opt::autosurgreal2) {
                                            auto local = sdk::GetGameLogic()->GetLocalPlayer();
                                            auto logic = sdk::GetGameLogic();
                                            auto tilemap = logic->GetTileMap();
                                            auto abab = logic->GetTileMap();
                                            static int x = 0, y = 0;
                                            static Tile* supermario = tilemap->GetTileSafe(x, y);
                                            if (local) {
                                                if (opt::autosurgreal2) {
                                                    //for (auto& tile : abab->tiles) {
                                                    //    if (tile.foreground != 8) {
                                                    //        if (tile.foreground == 4296) {
                                                    //            GameUpdatePacket legitpacket{ 0 };
                                                    //            legitpacket.type = PACKET_STATE;
                                                    //            legitpacket.vec_x = local->pos.x;
                                                    //            legitpacket.vec_y = local->pos.y;
                                                    //            GameUpdatePacket punchpacket{ 0 };
                                                    //            punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
                                                    //            punchpacket.int_data = 32;                     // punch id
                                                    //            punchpacket.vec_x = local->GetPos().x;
                                                    //            punchpacket.vec_y = local->GetPos().y;
                                                    //            punchpacket.int_x = tile.position_x;
                                                    //            punchpacket.int_y = tile.position_y;
                                                    //            legitpacket.int_data = 32;
                                                    //            legitpacket.int_x = tile.position_x;
                                                    //            legitpacket.int_y = tile.position_y;
                                                    //            legitpacket.flags = 2560;
                                                    //            opt::autosurgreal2 = false;
                                                    //            static types::time timepunch = std::chrono::system_clock::now();
                                                    //            if (utils::run_at_interval(timepunch, 0.5)) {
                                                    //                SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                                                    //            }

                                                    //        }
                                                    //    }
                                                    //}
                                                    auto pos = local->GetPos();
                                                    static int x, y;
                                                    x = pos.x / 32, y = pos.y / 32;
                                                    y += 2;
                                                    GameUpdatePacket punchpacket{ 0 };
                                                    punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
                                                    punchpacket.int_data = 4296;                     // punch id
                                                    punchpacket.int_x = x;
                                                    punchpacket.int_y = y;
                                                    punchpacket.pos_x = pos.x;
                                                    punchpacket.pos_y = pos.y;
                                                    opt::autosurgreal2 = false;
                                                    SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                                                    GameUpdatePacket punchpacket2{ 0 };
                                                    punchpacket2.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
                                                    punchpacket2.int_data = 32;                     // punch id
                                                    punchpacket2.int_x = x;
                                                    punchpacket2.int_y = y;
                                                    punchpacket2.pos_x = pos.x;
                                                    punchpacket2.pos_y = pos.y;
                                                    SendPacketRawHook::Execute(4, &punchpacket2, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                                                }
                                            }
                                        }
                                    }
                                    if (opt::surgery_bug) {
                                        gt::send(2, "action|dialog_return\ndialog_name|surge\n");
                                        variantlist_t va{ "OnTextOverlay" };
                                        va[1] = "`2Auto surgery working!";
                                        gt::send_varlist_self(va);
                                    }
                                }
                                if (content.find("YOUR MEDICAL LICENSE IS REVOKED") != -1) {
                                    opt::labkit = false;
                                    opt::findfix = false;
                                    opt::foundfix = false;
                                }
                                if (content.find("You can surg again.") != -1) {
                                    opt::labkit = false;
                                    opt::findfix = false;
                                    opt::foundfix = false;
                                    if (opt::autosurgreal) {
                                        opt::autosurgreal2 = true;
                                        if (opt::autosurgreal2) {
                                            auto local = sdk::GetGameLogic()->GetLocalPlayer();
                                            auto logic = sdk::GetGameLogic();
                                            auto tilemap = logic->GetTileMap();
                                            auto abab = logic->GetTileMap();
                                            static int x = 0, y = 0;
                                            static Tile* supermario = tilemap->GetTileSafe(x, y);
                                            if (local) {
                                                if (opt::autosurgreal2) {
                                                    //for (auto& tile : abab->tiles) {
                                                    //    if (tile.foreground != 8) {
                                                    //        if (tile.foreground == 4296) {
                                                    //            GameUpdatePacket legitpacket{ 0 };
                                                    //            legitpacket.type = PACKET_STATE;
                                                    //            legitpacket.vec_x = local->pos.x;
                                                    //            legitpacket.vec_y = local->pos.y;
                                                    //            GameUpdatePacket punchpacket{ 0 };
                                                    //            punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
                                                    //            punchpacket.int_data = 32;                     // punch id
                                                    //            punchpacket.vec_x = local->GetPos().x;
                                                    //            punchpacket.vec_y = local->GetPos().y;
                                                    //            punchpacket.int_x = tile.position_x;
                                                    //            punchpacket.int_y = tile.position_y;
                                                    //            legitpacket.int_data = 32;
                                                    //            legitpacket.int_x = tile.position_x;
                                                    //            legitpacket.int_y = tile.position_y;
                                                    //            legitpacket.flags = 2560;
                                                    //            opt::autosurgreal2 = false;
                                                    //            static types::time timepunch = std::chrono::system_clock::now();
                                                    //            if (utils::run_at_interval(timepunch, 2)) {
                                                    //                SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                                                    //            }
                                                    //        }
                                                    //    }
                                                    //}
                                                    auto pos = local->GetPos();
                                                    static int x, y;
                                                    x = pos.x / 32, y = pos.y / 32;
                                                    y += 2;
                                                    GameUpdatePacket punchpacket{ 0 };
                                                    punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
                                                    punchpacket.int_data = 4296;                     // punch id
                                                    punchpacket.int_x = x;
                                                    punchpacket.int_y = y;
                                                    punchpacket.pos_x = pos.x;
                                                    punchpacket.pos_y = pos.y;
                                                    opt::autosurgreal2 = false;
                                                    SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                                                    GameUpdatePacket punchpacket2{ 0 };
                                                    punchpacket2.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
                                                    punchpacket2.int_data = 32;                     // punch id
                                                    punchpacket2.int_x = x;
                                                    punchpacket2.int_y = y;
                                                    punchpacket2.pos_x = pos.x;
                                                    punchpacket2.pos_y = pos.y;
                                                    SendPacketRawHook::Execute(4, &punchpacket2, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if (opt::autofish) {
                                if (content.find("You caught a ```2") != -1) {
                                   opt::autofish2 = true;
                                }
                                else if (content.find("There was nothing on the line!") != -1) {
                                    opt::autofish2 = true;
                                }
                            }
                            //if (content.find("Unknown command.") != -1) {
                            //    return;
                            //}
                            if (opt::auto_ban) {
                                if (content.find("[SB]") != -1) {
                                    if (content.find(opt::cheat::ban_text) != -1) {
                                        std::string name = content.substr(content.find("[SB]_ `5** from (`0") + 19, content.length() - content.find("[SB]_ `5** from (`0") - 1);
                                        std::string namexd = name.substr(0, name.find("`````5) in "));
                                        utils::replace(namexd, "`#@", "");
                                        utils::replace(namexd, "`^", "");
                                        utils::replace(namexd, "`0", "");
                                        utils::replace(namexd, "`2", "");
                                        gt::send(2, "action|input\n|text|/info " + namexd);
                                        opt::info = true;
                                        if (opt::perma) {
                                            gt::send(2, "action|dialog_return\ndialog_name|punish_view\nbuttonClicked|ban_729\n\nreason|" + opt::cheat::reason);
                                        }
                                        else if (opt::ip_perma) {
                                            gt::send(2, "action|dialog_return\ndialog_name|punish_view\nbuttonClicked|ipban\n\nreason|");
                                        }
                                    }
                                }
                            }
                            /*
                            if (content.find("[SB]_") != -1) {
                                //CP:_OID:_CT:[W]_ `6<`wHZID42``>`` `$s``
                                utils::replace(content,"`","");
                                DiscordWebhookSender(content);
                            }
                            if (content.find("CP:0_PL:4_OID:_CT:[BC]_") != -1) {
                                utils::replace(content, "`", "");
                                DiscordWebhookSender(content);
                            }
                            if (content.find("CP:0_PL:4_OID:_CT:[CB]_") != -1) {
                                utils::replace(content, "`", "");
                                DiscordWebhookSender(content);
                            }
                            */
                        }
                        //`7[```wHZID3`` spun the wheel and got `436``!`7]``
                    }
                }

            } break;
            case PACKET_SEND_INVENTORY_STATE: {
                variantlist_t varlist{};
                auto extended = utils::get_extended(packet);
                break;
            }
            case PACKET_STATE: {

                break;
            }
            case PACKET_SET_CHARACTER_STATE: {
                if (!local)
                    break;

                
                if (packet->netid == local->netid) {
                    if (opt::cheat::antighost && global::state.gravity < packet->gravity_out && global::state.speed > packet->speed_out)
                        return;

                    global::state.copy_from_packet(packet);
                }
            } break;
        }

        orig(logic, packet);
    }
};