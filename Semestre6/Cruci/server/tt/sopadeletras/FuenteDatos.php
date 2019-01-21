<?php

/*******************
 * Our error reporting settings.
 *******************/

error_reporting(E_ALL ^ E_NOTICE ^ E_WARNING);

/*******************
 * FuenteDatos class.
 *******************/



class FuenteDatos {

    /*******************
     * The error var.
     *******************/

    public $error;

    /*******************
     * The connect var.
     *******************/

    private $conn;


    /*******************
     * Our MySQL host var.
     *******************/

    private $host = "xxxxxxxxxxxx";

    /*******************
     * Our MySQL username var.
     *******************/

    private $username = "xxxxxxxxxxxxxx";

    /*******************
     * Our MySQL password var.
     *******************/

    private $password = "xxxxxxxxxxxxxxxxxxx";

    /*******************
     * Our MySQL Database Name var.
     *******************/

    private $db_name = "xxxxxxxxxxxxxxxxxx";

    /*******************
     * Our database var.
     *******************/

    private $db;

    /*******************
     * Our connection status var.
     *******************/

     private $connected = FALSE;

     /*******************
     *Path to the error image!.
     *******************/

     public $image_path = "imagenes/iconos/";


    /*******************
     * This is called when the class is called, we will connect to our database here.
     *******************/
   public function  __construct(){

        $style = "<style type=\"text/css\">
        .info
        {
            BORDER-RIGHT: #000 1px solid;
            PADDING-RIGHT: 6px;
            BORDER-TOP: #000 1px solid;
            PADDING-LEFT: 6px;
            PADDING-BOTTOM: 6px;
            BORDER-LEFT: #000 1px solid;
            COLOR: #000;
            PADDING-TOP: 6px;
            BORDER-BOTTOM: #000 1px solid;
            BACKGROUND-COLOR: #fcf8c7;
            TEXT-ALIGN: left;
            -moz-border-radius: 5px
        }
        .info IMG
        {
            BORDER-RIGHT: 0px;
            BORDER-TOP: 0px;
            MARGIN-TOP: 10px;
            FLOAT: left;
            VERTICAL-ALIGN: middle;
            BORDER-LEFT: 0px;
            MARGIN-RIGHT: 5px;
            BORDER-BOTTOM: 0px
        }
        .info H2
        {
            PADDING-RIGHT: 0px;
            PADDING-LEFT: 0px;
            PADDING-BOTTOM: 0px;
            MARGIN: 0px 0px 0px 60px;
            PADDING-TOP: 0px
        }
        .info P
        {
            PADDING-RIGHT: 0px;
            PADDING-LEFT: 20px;
            PADDING-BOTTOM: 0px;
            MARGIN: 0px 0px 0px 60px;
            PADDING-TOP: 0px
        }
        .info H2
        {
            FONT-SIZE: 14px;
            PADDING-BOTTOM: 4px;
            BORDER-BOTTOM: #000 1px solid
        }
        </style>";

        $message["start"] = "<title>MySQL Error!</title></head><body><fieldset class=\"info\"><img src=\"".$this->image_path."process-stop.png\"><h2>MySQL Error!</h2><p><br/>";
        $message["end"] = "</p></fieldset>";
        $this->error['connection'] = $style.$message["start"]."<b>There has been an error while trying to connect to your MySQL database.</b>".$message["end"];
        $this->error['query'] = $style.$message["start"]."<b>There has been an error while performing your mysql query.</b>".$message["end"];
        $this->error['database'] = $style.$message["start"]."<b>there has been an error, your database could not be found.</b>".$message["end"];

        $this->conn = @mysql_connect($this->host, $this->username, $this->password);

        if(!$this->conn){
            echo $this->error['connection'];
            die();
        }else{
            $this->db = @mysql_select_db($this->db_name);
            if (!$this->db){
                echo $this->error['database'];
            }else{
                $this->connected = TRUE;
                return(true);
            }
        }

    }

    /*******************
     * Our MySQL_QUERY function.
     *******************/

    public function getDatos($sql){
        $query = mysql_query($sql) or die("ERROR AL EJECUTAR CONSULTA");

        if(!$query){
            echo $this->error['query'];
            return (false);
        }else{
            return $query;
        }
    }

    public function insert($sql){
        return $this->getDatos($sql);
    }









    /*******************
     * Our MySQL_NUM_ROWS function.
     *******************/

    public function num_rows($sql){
        $query = mysql_query($sql);

        if(!$query){
            echo $this->error['query'];
            return (false);
        }else{
            $num_rows = mysql_num_rows($query);
            return ($num_rows);
        }
    }

    /*******************
     * Our MySQL_FETCH_ASSOC function.
     *******************/

    public function assoc($sql){

        $query = mysql_query($sql);

        if(!$query){
            echo $this->error['query'];
        }else{
            $assoc = mysql_fetch_assoc($query)or die(mysql_error());

            if(!$assoc){
                echo $this->error['query'];
            }else{
                return ($assoc);
        }
    }
    }

    /*******************
     * Our MySQL_CLOSE function.
     *******************/

    public function close(){

        if($this->connected = TRUE){
            mysql_close($this->conn)or die(mysql_error());
            $this->connected = FALSE;
        }
    }

    /*******************
     * We end our class here.
     *******************/

    public function __destruct(){
        $this->close();
    }

}

/*******************
 *Our class is over.
*******************/
?>