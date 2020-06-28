﻿CREATE TABLE userinfo
(
    id CHAR(16) NOT NULL PRIMARY KEY,
    username CHAR(24) NOT NULL,
    password CHAR(32) NOT NULL,
    logintime DATETIME NOT NULL,
    online TINYINT(1) NOT NULL,
    secretkey CHAR(20),
    heartbeat DATETIME
);

CREATE TABLE petprofile
(
    id CHAR(16) NOT NULL PRIMARY KEY,
    username CHAR(24) NOT NULL,
    level SMALLINT UNSIGNED NOT NULL DEFAULT 0,
    age INT UNSIGNED NOT NULL DEFAULT 0,
    growth INT UNSIGNED NOT NULL DEFAULT 0,
    food SMALLINT UNSIGNED NOT NULL DEFAULT 5200,
    clean SMALLINT UNSIGNED NOT NULL DEFAULT 5200,
    health TINYINT UNSIGNED NOT NULL DEFAULT 10,
    mood SMALLINT UNSIGNED NOT NULL DEFAULT 100,
    growth_speed TINYINT UNSIGNED NOT NULL DEFAULT 1,
    status INT UNSIGNED NOT NULL DEFAULT 0,
    online_time INT UNSIGNED NOT NULL DEFAULT 0,
    tuotuo INT DEFAULT 0
);

CREATE TABLE shopinfo
(
    itemname CHAR(18) NOT NULL PRIMARY KEY,
    price SMALLINT NOT NULL,
    descrpition VARCHAR(32),
    type TINYINT UNSIGNED NOT NULL
);

CREATE TABLE ownitems
(
    itemname CHAR(18) NOT NULL,
    quantity SMALLINT UNSIGNED NOT NULL,
    type TINYINT UNSIGNED NOT NULL,
    id CHAR(16) NOT NULL,
    PRIMARY KEY (itemname, id)
);

INSERT INTO userinfo VALUES("999", "miantuotuo", "123", NOW(), 0, NULL, NOW());
INSERT INTO petprofile(id, username) VALUES("999", "miantuotuo");
INSERT INTO shopinfo VALUES("lanxingtangguo", "80", "star-shaped blue candy", 0);
INSERT INTO ownitems VALUES("lanxingtangguo", 1, 0, "999");
