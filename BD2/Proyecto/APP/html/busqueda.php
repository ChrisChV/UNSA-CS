<html>
 <head>
  <title>Prueba de PHP</title>
 </head>
 <body>
<form action="busqueda.php" method="post">
 <p>Busqueda: <input type="text" name="word" /></p>
 <p><input type="submit" /></p>
</form>
 

<?php 
$link = mysqli_connect('localhost','root', 'root')
    or die('No se pudo conectar: ' . mysql_error());
mysqli_select_db($link,'Google') or die('No se pudo seleccionar la base de datos');

$query = "SELECT idWord FROM Word where Word = '" . $_POST["word"] . "'";
$result = mysqli_query($link,$query) or die('Consulta fallida: ' . mysql_error());
$n = mysqli_num_rows($result);
if ($n == 0) {
	echo "No existe la palabra";
}
else{
	$row = mysqli_fetch_array($result);
	$id = $row[0];
	$starttime = microtime(true);
	$query = "CALL SEARCH(" . $id . ")";
	$result = mysqli_query($link,$query) or die('Consulta fallida: ' . mysql_error());
	$endtime = microtime(true);
	$duration = $endtime - $starttime;
	echo "Palabra Buscada: " . $_POST["word"];
	echo "\n";
	echo "Resultados traidos en " . $duration . " segundos"; 
	echo "\n";
	$n = mysqli_num_rows($result);

	if ($n == 0){
		echo "No existen Relaciones";
	}
	else{
		echo "<table>\n";
		while ($line = mysqli_fetch_array($result)) {
			echo"<tr>\n";
			echo "<td>";
		    echo ($line[0]);
		    echo "</td>\n";
		    echo "<td>";
		    echo ($line[1]);
		    echo "</td>\n";
		    echo "</tr>\n";
		}
		echo "</table>\n";	
	}
	
}

mysqli_free_result($result);

mysqli_close($link);

?>

</body>
</html>

