CREATE TABLE userinfo
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
    tuotuo INT NOT NULL DEFAULT 1000
);

CREATE TABLE shopinfo
(
    itemname CHAR(18) NOT NULL PRIMARY KEY,
    price SMALLINT NOT NULL,
    descrpition VARCHAR(32),
    type TINYINT UNSIGNED NOT NULL,
    food SMALLINT NOT NULL DEFAULT 0,
    clean SMALLINT NOT NULL DEFAULT 0,
    health SMALLINT NOT NULL DEFAULT 0
);

CREATE TABLE ownitems
(
    itemname CHAR(18) NOT NULL,
    quantity SMALLINT UNSIGNED NOT NULL,
    type TINYINT UNSIGNED NOT NULL,
    id CHAR(16) NOT NULL,
    PRIMARY KEY (itemname, id)
);

CREATE TABLE jobsinfo
(
    name CHAR(18) NOT NULL PRIMARY KEY,
    wage SMALLINT NOT NULL,
    lowestLevel TINYINT UNSIGNED NOT NULL,
    eduRestrictions CHAR(128) NOT NULL
);

CREATE TABLE workinginfo
(
    id CHAR(16) NOT NULL,
    job CHAR(18) NOT NULL,
    begintime DATETIME NOT NULL,
    count INT UNSIGNED NOT NULL DEFAULT 0,
    working TINYINT(1) NOT NULL,
    PRIMARY KEY (id, job)
);

INSERT INTO userinfo VALUES("999", "miantuotuo", "123", NOW(), 0, NULL, NOW());
INSERT INTO petprofile(id, username) VALUES("999", "miantuotuo");
INSERT INTO shopinfo VALUES("lanxingtangguo", "80", "star-shaped blue candy", 0, 0, 0, 0);
INSERT INTO shopinfo VALUES("qimiaoyuyan", "80", "wonderful bath salt", 0, 0, 0, 0);
INSERT INTO ownitems VALUES("lanxingtangguo", 1, 0, "999");
INSERT INTO ownitems VALUES("qimiaoyuyan", 1, 0, "999");
INSERT INTO jobsinfo VALUES("cazhuantou", 10, 0, "none");
INSERT INTO workinginfo VALUES("999", "cazhuantou", NOW(), 0, 0);
