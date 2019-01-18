CREATE TABLE `sopas` (
`identificador` int( 11 ) NOT NULL AUTO_INCREMENT ,
`titulo` varchar( 100 ) NOT NULL ,
`palabras` varchar( 2000 ) NOT NULL ,
`trampas` varchar( 2000 ) NOT NULL ,
`filas` int( 11 ) NOT NULL ,
`columnas` int( 11 ) NOT NULL ,
`fecha` date NOT NULL ,
PRIMARY KEY ( `identificador` )
) ENGINE = MYISAM DEFAULT CHARSET = utf8 AUTO_INCREMENT =377;