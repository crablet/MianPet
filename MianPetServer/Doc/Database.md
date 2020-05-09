# userinfo
## 用于处理登录信息
|     id    |username|password|logintime|  online  |
|:---------:|:------:|:------:|:-------:|:--------:|
|  CHAR(32) |CHAR(16)|CHAR(32)| DATETIME|TINYINT(1)|
|  NOT NULL |NOT NULL|NOT NULL| NOT NULL| NOT NULL |
|PRIMARY KEY|        |        |         |          |
### 注：在MySQL中用tinyint(1)代替boolean，MySQL并无真实的boolean类型
 
# petprofile
## 宠物实时数据
|     id    |username|      level      |    age     |   growth   |      food       |     clean       |     health     |      mood       |  growth_speed  | status | online_time|
|:---------:|:------:|:---------------:|:----------:|:----------:|:---------------:|:---------------:|:--------------:|:---------------:|:--------------:|:------:|:----------:|
|  CHAR(32) |CHAR(16)|SMALLINT UNSIGNED|INT UNSIGNED|INT UNSIGNED|SMALLINT UNSIGNED|SMALLINT UNSIGNED|TINYINT UNSIGNED|SMALLINT UNSIGNED|TINYINT UNSIGNED|CHAR(24)|INT UNSIGNED|
|  NOT NULL |NOT NULL|     NOT NULL    |  NOT NULL  |  NOT NULL  |     OT NULL     |    NOT NULL     |    NOT NULL    |     NOT NULL    |    NOT NULL    |NOT NULL|  NOT NULL  |
|PRIMARY KEY|        |                 |            |            |                 |                 |                |                 |                |        |            |
