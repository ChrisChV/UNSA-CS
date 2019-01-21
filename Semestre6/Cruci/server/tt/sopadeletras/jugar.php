<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>

<body>





<div class="content">
	<div class="bloque">
		<div class="titulo">Juego Sopa de Letras</div>
			<ul>
				<li><a href="modulos/juegos/sopadeletras/index.php">Crear una Sopa de letras</a></li>
				<li><a href="modulos/juegos/sopadeletras/seleccionarSopa.php">Jugar a otras sopas de letras</a></li>
			</ul>
	</div>
</div>



<div class="content">
	<div class="bloque">
					<div class="titulo">Juego Sopa de Letras</div>
					<div class="contenido">


<?php


/*Recuperamos los parametros de la nueva sopa*/
$idSopa = $_GET["idSopa"];

include_once $_SERVER["DOCUMENT_ROOT"] . "/src/juegos/SopaLetras.php";
$gestorSopas = new SopaLetras();

$gestorSopas->setIdentificador($idSopa);
$gestorSopas->mostrarSopaLetras();

?>


				</div>
   </div>


</div>




</body>
</html>




