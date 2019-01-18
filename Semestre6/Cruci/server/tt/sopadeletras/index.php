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

					<div class="contenido">
					<div class="titulo">Crea tu propia sopa de letras</div>
<script>



function addOptions(campoSelect, texto, valor){

	if (valor==""){
		alert("Introduzca alguna palabra");
	}else{
		campoSelect.options[campoSelect.length] = new Option(texto, valor);
	}
}


function controlSubmit(){

	if (document.formulario.titulo.value==""){
		alert("Introduzca un titulo para la sopa de letras");
		return false;
	}


    /*Seleccionamos todas las trampas*/
	for(i=0;i<=document.formulario.elements[8].length-1;i++) {
		document.formulario.elements[8].options[i].selected = true;
	}


	/*Seleccionamos todas las palabras*/
	for(i=0;i<=document.formulario.elements[7].length-1;i++) {
		document.formulario.elements[7].options[i].selected = true;
	}

 return true;
}
</script>



<form name="formulario" action="modulos/juegos/sopadeletras/generarSopa.php" method="POST" onSubmit="return controlSubmit();">
<table>
	<tr>
		<td coslpan="2">Titulo <input type="text" name="titulo" /></td>
	</tr>
	<tr>
	<td>Columnas</td>
	<td><select name="columnas">
			<option value="10">10</option>
			<option value="11">11</option>
			<option value="12">12</option>
			<option value="13">13</option>
			<option value="14">14</option>
			<option value="15">14</option>
		</select>
	</td>
</tr>
<tr>
	<td>Filas</td>
	<td>
		<select name="filas">
			<option value="10">10</option>
			<option value="11">11</option>
			<option value="12">12</option>
			<option value="13">13</option>
			<option value="14">14</option>
			<option value="15">14</option>
		</select>
	</td>
</tr>
<tr>
	<td>Palabras <br/>
		<input type="text" maxlength="10" value="" name="nuevaPalabra"/>
		<input type="button" value="+" onClick="addOptions(document.formulario.elements[7], document.formulario.nuevaPalabra.value, document.formulario.nuevaPalabra.value);document.formulario.nuevaPalabra.value='';" />
	</td>
	<td>Trampas <br/>
		<input type="text" maxlength="10" value="" name="nuevaTrampa"/>
		<input type="button" value="+" onClick="addOptions(document.formulario.elements[8], document.formulario.nuevaTrampa.value, document.formulario.nuevaTrampa.value);document.formulario.nuevTrampa.value='';" />
	</td>
</tr>
<tr>
	<td><select style="width:250px;" name="palabras[]" multiple size="10"></select></td>
	<td><select style="width:250px;" name="trampas[]" multiple size="10" ></select></td>
</tr>
</table>

<input type="submit" value="Generar Sopa" />

</form>




				</div>
   </div>


</div>




</body>
</html>
