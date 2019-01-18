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

include_once $_SERVER["DOCUMENT_ROOT"] . "/src/juegos/SopaLetras.php";
$gestorSopas = new SopaLetras();

$listadoSopas  = $gestorSopas->getSopasExistentes();
$numeroSopas   = count($listadoSopas);

echo "<table align='center' border='1'>";
echo "<tr><td colspan='3'>Seleccione una sopa de letras</td></tr>";

for ($i= 0 ; $i < $numeroSopas ; $i++){
	$titulo =  $listadoSopas[$i]->getTitulo();
	$idSopa =  $listadoSopas[$i]->getIdentificador();

echo <<< MARCA
	<tr>
		<td width="150"><a href="modulos/juegos/sopadeletras/jugar.php?idSopa=$idSopa">$titulo</a></td>
	</tr>
MARCA;
}



echo "</table>";




?>
				</div>
   </div>


</div>



</body>
</html>




