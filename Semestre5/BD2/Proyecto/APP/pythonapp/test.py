$link = mysqli_connect('localhost','root', 'root')
    or die('No se pudo conectar: ' . mysql_error());
echo "correcto" ;
mysql_select_db('Google') or die('No se pudo seleccionar la base de datos');

$query = "select * from temp";
$result = mysql_query($link,$query) or die('Consulta fallida: ' . mysql_error());

echo "<table>\n";
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    echo "\t<tr>\n";
    foreach ($line as $col_value) {
        echo "\t\t<td>$col_value</td>\n";
    }
    echo "\t</tr>\n";
}
echo "</table>\n";

mysql_free_result($result);

mysql_close($link);