
<?php 
$link = mysqli_connect('localhost','root', 'root')
    or die('No se pudo conectar: ' . mysql_error());
mysqli_select_db($link,'ml') or die('No se pudo seleccionar la base de datos');
$starttime = microtime(true);
$query = "CALL recommend( " .$_POST["word"]. ",10)";
$result = mysqli_query($link,$query) or die('Consulta fallida: ' . mysql_error());
$endtime = microtime(true);
$duration = $endtime - $starttime;
echo "Resultados traidos en " . $duration . " segundos"; 
$n = mysqli_num_rows($result);
if ($n == 0){
	echo "No existen Recomendaciones";
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
	    echo "<td>";
	    echo ($line[2]);
	    echo "</td>\n";
	    echo "</tr>\n";
	}
	echo "</table>\n";	
}

mysqli_free_result($result);
mysqli_close($link);
?>
