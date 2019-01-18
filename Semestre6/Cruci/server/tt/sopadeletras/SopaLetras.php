<?php
require_once  "../../../src/datos/FuenteDatos.php";
error_reporting(E_ALL ^ E_NOTICE ^ E_WARNING);



class SopaLetras {


	private $identificador;
	private $titulo;
	private $palabras;
	private $trampas;
	private $filas;
	private $columnas;
	private $fecha;

	private $sopa;




    public function setIdentificador($identificador){

            $this->identificador= $identificador;
    }

  	public function getIdentificador(){
         return   $this->identificador;
    }


    public function setTitulo($titulo){
            $this->titulo= $titulo;
    }

  	public function getTitulo(){
         return   $this->titulo;
    }

    public function setPalabras($palabras){
            $this->palabras= $palabras;
    }

  	public function getPalabras(){
         return   $this->palabras;
    }

    public function setTrampas($trampas){
            $this->trampas= $trampas;
    }

  	public function getTrampas(){
         return   $this->trampas;
    }


    public function setFilas($filas){
            $this->filas= $filas;
    }

  	public function getFilas(){
         return   $this->filas;
    }


    public function setColumnas($columnas){
            $this->columnas= $columnas;
    }

  	public function getColumnas(){
         return   $this->columnas;
    }

    public function setFecha($fecha){
            $this->fecha= $fecha;
    }

  	public function getFecha(){
         return   $this->fecha;
    }





    public function __construct(){

    	/*Estructura de la sopa*/
    	$this->sopa = array();

    }
	public function __destruct(){  }




   public function crearSopa(){

		$titulo   	= $this->getTitulo();
		$palabras	= $this->getPalabras();
		$trampas	= $this->getTrampas();
		$filas		= $this->getFilas();
		$columnas	= $this->getColumnas();


		$lasPalabras =  implode(";",$palabras);
		$lasTrampas  =  implode(";",$trampas);

 		$fuenteDatos  = new FuenteDatos();
		$consulta     = "INSERT INTO sopas (titulo, palabras, trampas, filas, columnas, fecha) VALUES ('$titulo','$lasPalabras','$lasTrampas','$filas','$columnas', NOW())" ;

		$resulset     = $fuenteDatos->insert($consulta);


		/*Conocemos el id de la ultima sopa introducida*/
		$consulta = "SELECT LAST_INSERT_ID() AS lastInsertID FROM sopas LIMIT 0,1";


		/*Recuperamos el id del ultimo tema introducido*/
		$resulset       = $fuenteDatos->getDatos($consulta);
		while($row = mysql_fetch_array($resulset)){
			   $ultimaSopaId = $row['lastInsertID'];
		}

		return $ultimaSopaId;


    }
/*****************************************************************************/






public function mostrarSopaLetras(){


		$unaSopa = $this->getSopaLetras($this->identificador);

		$this->titulo  	= $unaSopa->getTitulo();
		$this->palabras	= $unaSopa->getPalabras();
		$this->trampas	= $unaSopa->getTrampas();
		$this->filas	= $unaSopa->getFilas();
		$this->columnas	= $unaSopa->getColumnas();
		$this->sopa     = Array();


		$abecedario = array("A","B","C","D","E","F","G","H","I","J","K","L","M","N","&Ntilde;","O","P","Q","R","S","T","U","V","W","X","Y","Z");


		/*Inicializamos la sopa con letras aleatorias*/
		for ($i=0; $i< $this->filas;$i++){
			$this->sopa[$i] = array();
			for ($j=0; $j<$this->columnas; $j++){
				$letraRandom = mt_rand(0,(count($abecedario)-1));
				$letra       = $abecedario[$letraRandom];
				$this->sopa[$i][$j] = $letra;
			}//for $j
		}//for $i


		/*Introducimos las palabras de forma aleatoria*/
		for ($i=0;$i<count($this->palabras	);$i++){
			 $this->meterPalabra($this->palabras[$i]);
		}//for $i



		/*Introducimos las trampas de forma aleatoria*/
		for ($t=0;$t<count($this->trampas);$t++){
 			$this->meterPalabra($this->trampas[$t]);
  		}//for $t


		$this->sopaMayusculas();
		$this->pintarSopa();


		return $this->sopa;
}








/*
  Devuelve la forma aleatoria en la que aparecerá la nueva palabra en la sopa
*/
public  function getForma(){

	$direcciones = array ('H','V','I','D');  /*   - | / \    */
	$sentidos 	 = array ('D','I');

	$dirRandom  = mt_rand(0,3);
	$sentRandom = mt_rand(0,1);

	$direccion  = $direcciones[$dirRandom];
	$sentido 	= $sentidos[$sentRandom];

	$forma = array($direccion,$sentido);
	return $forma;

}//dameFormas







/*
 Devuelve una coordenada aleatoria para introducir una nueva palabra
*/
function getCoordenadas($forma,$palabra){

	$longitud  = strlen($palabra);
	$direccion = $forma[0];
	$sentido   = $forma[1];


	$dimensionX = $this->filas;
	$dimensionY = $this->columnas;


	$seguir = TRUE;
	while ($seguir){
		$x = mt_rand(0,$dimensionX-1);//echo $x;
		$y = mt_rand(0,$dimensionY-1);//echo $y;
		$coordenadas = array($x,$y);

		switch ($direccion){
			case 'H'://-
			    if ((($y+$longitud)<$dimensionY) && ($this->estaLibre($palabra,$coordenadas,$direccion,$sentido))) $seguir = FALSE;
			    break;
			case 'V'://|
			    if ((($x+$longitud)<$dimensionX) && ($this->estaLibre($palabra,$coordenadas,$direccion,$sentido))) $seguir = FALSE;
			    break;
			case 'I'://\
			    if (((($x+$longitud-1)<($dimensionX)) && (($y+$longitud-1)<($dimensionY))) && ($this->estaLibre($palabra,$coordenadas,$direccion,$sentido))) $seguir = FALSE;
			    break;
			case 'D':///
			    if (((($x-$longitud+1)>=0) && (($y+$longitud-1)<$dimensionY)) && ($this->estaLibre($palabra,$coordenadas,$direccion,$sentido))) $seguir = FALSE;
			    break;
		}//switch
	}//while
	$coordenadas = array($x,$y);
	return $coordenadas;
}//dameCoordenadas









/*
Indica si una palabra puede entrar en la sopa de letras
para una un sentido y  direccion dados a partir de una coordenada
*/
function estaLibre($palabra,$coordenadas,$direccion,$sentido){
	$x = $coordenadas[0];
	$y = $coordenadas[1];
	$seguir = TRUE;
	switch ($direccion){
		case 'H'://-
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$c++,$z++){
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
				}
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $y = $y + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$c--,$z++)
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
		    }//INDIRECTO
		    break;
		case 'V'://|
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$f++,$z++)
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $x = $x + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$f--,$z++)
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
		    }//INDIRECTO
		    break;
		case 'I'://\
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$c++,$f++,$z++)
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $x = $x + strlen($palabra) - 1;
		        $y = $y + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$c--,$f--,$z++)
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
		    }//INDIRECTO
		    break;
		case 'D':///
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$c++,$f--,$z++)
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $x = $x - strlen($palabra) + 1;
		        $y = $y + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;($z<(strlen($palabra)) && $seguir);$c--,$f++,$z++)
		            if ((ctype_lower($this->sopa[$f][$c]))) $seguir = FALSE;
		    }//INDIRECTO
		    break;
	}//switch
	return $seguir;
}//estaLibre










/*
Introduce una nueva palabra en la sopa de letras
*/

function meterPalabra($palabra){


	$forma     = $this->getForma();
	$direccion = $forma[0];
	$sentido   = $forma[1];

	$coordenadas = $this->getCoordenadas($forma,$palabra);
	$x = $coordenadas[0];
	$y = $coordenadas[1];


	switch ($direccion){
		case 'H'://-
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$c++,$z++){
		            $this->sopa[$f][$c] = $palabra[$z];
				}
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $y = $y + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$c--,$z++)
		            $this->sopa[$f][$c] = $palabra[$z];
		    }//INDIRECTO
		    break;
		case 'V'://|
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$f++,$z++)
		            $this->sopa[$f][$c] = $palabra[$z];
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $x = $x + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$f--,$z++)
		            $this->sopa[$f][$c] = $palabra[$z];
		    }//INDIRECTO
		    break;
		case 'I'://\
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$c++,$f++,$z++)
		            $this->sopa[$f][$c] = $palabra[$z];
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $x = $x + strlen($palabra) - 1;
		        $y = $y + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$c--,$f--,$z++)
		            $this->sopa[$f][$c] = $palabra[$z];
		    }//INDIRECTO
		    break;
		case 'D':///
		    if ($sentido == 'D'){
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$c++,$f--,$z++)
		            $this->sopa[$f][$c] = $palabra[$z];
		    }//DIRECTO
		    if ($sentido == 'I'){
		        $x = $x - strlen($palabra) + 1;
		        $y = $y + strlen($palabra) - 1;
		        for ($f=$x,$c=$y,$z=0;$z<(strlen($palabra));$c--,$f++,$z++)
		            $this->sopa[$f][$c] = $palabra[$z];
		    }//INDIRECTO
		    break;
	}//switch
	return $this->sopa;
}//meterPalabras











/*
 Muestra la sopa de letras a modo de debug
*/

function pvShowSopa($sopa){
	$filas = count($sopa);
	echo  "<table border='2' >";
	for ($i=0; $i< $filas;$i++){
			$columnas = count($sopa[$i]);
			echo "<tr>";
			for ($j=0; $j<$columnas; $j++){
				echo "<td>".$sopa[$i][$j]."</td>";
			}//for $j
			echo "</tr>\n";
		}//for $i
	echo  "</table>";
}







function sopaMayusculas (){
	for ($i=0; $i<count($this->sopa); $i++)
	  for ($j=0; $j<count($this->sopa[$i]); $j++)
	    if ($this->sopa[$i][$j] != "&Ntilde;"){
	    	    $this->sopa[$i][$j] = strtoupper($this->sopa[$i][$j]);
	    }
}//sopaMay









public function getSopasExistentes(){

	$fuenteDatos  = new FuenteDatos();
	$consulta     = "SELECT * FROM sopas  order by titulo" ;
	$resulset     = $fuenteDatos->getDatos($consulta);

	$listadoSopas = array();

    while($row = mysql_fetch_array($resulset)){

		 $unaSopa = new SopaLetras();
		 $unaSopa->setIdentificador( $row['identificador'] );
         $unaSopa->setTitulo       ( $row['titulo'] );

         $listadoSopas[]=$unaSopa;
      }
      return $listadoSopas;
    }






public function getSopaLetras($identificador){

	$fuenteDatos  = new FuenteDatos();
	$consulta     = "SELECT * FROM sopas WHERE identificador = '$identificador' LIMIT 0,1" ;
	$resulset     = $fuenteDatos->getDatos($consulta);

	$unaSopa = new SopaLetras();

    while($row = mysql_fetch_array($resulset)){

	    $unaSopa->setIdentificador( $row['identificador'] );
        $unaSopa->setTitulo       ( $row['titulo'] );
		$unaSopa->setFecha        ( $row['fecha'] );
		$unaSopa->setPalabras     ( explode(";",$row['palabras']) );
		$unaSopa->setTrampas      ( explode(";",$row['trampas']) );
		$unaSopa->setFilas        ( $row['filas'] );
		$unaSopa->setColumnas     ( $row['columnas'] );



      }
      return $unaSopa;
    }










function pintarSopa(){


echo <<< MARCA

<script type="text/javascript" language="javascript" src="modulos/juegos/sopadeletras/funSopa.js"></script>



  <table align="center" frame="box" rules="alls" border="1" cellspacing="0" cellpadding="2" class="tablaSopa" style="z-index:2;">

MARCA;

		$reconoce = "";
		$sopa = $this->sopa;

		$matriz   = "var sopa = new Array();\n";
		for ($i=0;$i<$this->filas;$i++){
				echo "<tr>\n";
				$matriz.= "sopa[$i] = new Array();\n";
				for ($j=0;$j<$this->columnas;$j++){
					echo "<td id=\"letra_".$i."_".$j."\" value=\"".$sopa[$i][$j]."\" style=\"text-align:center;width:20;height:20;cursor:pointer;cursor:hand;\" onMouseDown=\"marcarSopa($i,$j)\" class=\"celdasSopa\">".$sopa[$i][$j]."</td>";
					$reconoce.= "ReconoceCapas('letra_".$i."_".$j."');\n";
					$matriz.= "sopa[$i][$j] = '".$sopa[$i][$j]."';\n";
				}//for $j
				echo "</tr>\n";
		}//for $i

echo <<< MARCA
	</table>
	<script language="JavaScript" type="text/javascript">
MARCA;


	echo $matriz;
	echo $reconoce;
	echo "filas   =". $this->filas .";";
	echo "columnas=". $this->filas .";";

echo <<< MARCA
	</script>
	<form name="formulario">

	<center>
		 <input type="button" value="Limpiar Sopa" onClick="borrar($this->filas, $this->columnas);" />
	</center>

	</form>
MARCA;




}








}
?>