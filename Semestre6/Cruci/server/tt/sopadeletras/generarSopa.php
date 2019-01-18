<?php
session_start();
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
<?php include "../../../plantillas/head.php"; ?>
<body onload="hacerArrastrables();">
<?php include "../../../plantillas/cabecera.php"; ?>




<div class="content">
	<div class="bloque">
		<div class="titulo">Juego Sopa de Letras</div>
			<ul>
				<li><a href="modulos/juegos/sopadeletras/index.php">Crear una Sopa de letras</a></li>
				<li><a href="modulos/juegos/sopadeletras/jugar.php">Jugar a otras sopas de letras</a></li>
			</ul>
	</div>
</div>



<div class="content">
	<div class="bloque">
					<div class="titulo">Juego Sopa de Letras</div>
					<div class="contenido">


<?php


/*Recuperamos los parametros de la nueva sopa*/
$palabras = $_POST["palabras"];
$trampas  = $_POST["trampas"];
$titulo   = $_POST["titulo"];
$filas    = $_POST["filas"];
$columnas = $_POST["columnas"];


include_once $_SERVER["DOCUMENT_ROOT"] . "/src/juegos/SopaLetras.php";
$gestorSopas = new SopaLetras();


$gestorSopas->setPalabras($palabras);
$gestorSopas->setTrampas($trampas);
$gestorSopas->setTitulo($titulo);
$gestorSopas->setFilas($filas);
$gestorSopas->setColumnas($columnas);


$idNuevaSopa = $gestorSopas->crearSopa();

$gestorSopas->setIdentificador($idNuevaSopa);
$gestorSopas->mostrarSopaLetras();


?>

				</div>
   </div>


</div>



<?php include "../../../plantillas/frameIzquierda.php"; ?>
<?php include "../../../plantillas/frameDerecha.php"; ?>
<?php include "../../../plantillas/pie.php"; ?>
</body>
</html>




