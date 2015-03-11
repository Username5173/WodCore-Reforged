UPDATE `creature_template` SET gossip_menu_id = 16986, `npcflag`=`npcflag`|17, trainer_type=2 WHERE `entry`=79892;
DELETE FROM `npc_trainer` WHERE `entry`=79892;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(79892, 7620, 100, 356, 0, 1), 
(79892, 7731, 500, 356, 50, 1), 
(79892, 7732, 10000, 356, 125, 1), 
(79892, 18248, 25000, 356, 200, 1), 
(79892, 33095, 100000, 356, 275, 1), 
(79892, 51294, 150000, 356, 350, 1), 
(79892, 88868, 250000, 356, 425, 1), 
(79892, 110410, 300000, 356, 500, 1);

UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=79971;
DELETE FROM `npc_vendor` WHERE `entry` = 79971 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(79971, 0, 6256, 0, 0, 0, 1), 
(79971, 0, 6365, 1, 3600, 0, 1), 
(79971, 0, 6529, 0, 0, 0, 1), 
(79971, 0, 6530, 0, 0, 0, 1), 
(79971, 0, 6532, 0, 0, 0, 1), 
(79971, 0, 6533, 2, 3600, 0, 1);

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-64 AND faction_index = 0;
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','79892','4.10929','-1.38352','0.583206','0.213478');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','44880','2.56324','5.39814','4.2152','0.408808');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','44880','1.28986','-0.327592','8.7592','6.09967');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','79971','-0.343123','7.68761','0.713196','0.655077');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','62953','25.7635','-22.1997','6.16921','1.42364');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','44880','2.56324','5.39814','5.7152','0.408808');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','-232440','8.77231','3.06427','0.000198','4.7124');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','0','-230764','1.15135','7.00761','0.089203','0.633848');
