var NS7=(document.getElementById && !document.all)?1:0;
function ReconoceCapas(capitas){//alert(capitas);
     var cadena="";
     if (NS7)  {
          cadena=capitas+"=document.getElementById('"+capitas+"')";
    	  eval (cadena);
        }
}




var estadoSopa = 0;
var numLetra   = 0;
var laCoordenadaX;
var laCoordenadaY;
var direccion;
var sentido;
var colores     = new Array('00','33','66','99','CC','FF');
var colorActual = generaColor();





function randomEspanol(max){
    var ahora = new Date();
    return((Math.round(Math.abs(Math.sin(ahora.getTime()))*1000000000)%max));
}//randomEspanol




function generaColor(){
	var colorcito = "#"+colores[Math.round(Math.random()*5)]+colores[Math.round(Math.random()*5)]+colores[Math.round(Math.random()*5)];
	return (colorcito);
}//generaColor









function marcarSopa(fila,columna){

  numSeccion= "";

  if (estadoSopa == 0){
	var campoFormulario = "coordenadas"+numSeccion+"_"+numLetra;
	//if (getValue(nombreFormulario,campoFormulario)!='')
	//	borrarSopa(getValue(nombreFormulario,campoFormulario),numSeccion);
	eval("letra"+numSeccion+"_"+fila+"_"+columna+".style.background = '"+colorActual+"';");
	estadoSopa = 1;
	laCoordenadaX = fila;
	laCoordenadaY = columna;
	//setValue(nombreFormulario,campoFormulario,implode(",",new Array(fila,columna)));
  }else{

	var valor = "";
	//Averiguamos la dirección y sentido de la palabra

	//Izquierda Directo
	if ((fila>laCoordenadaX) && (columna>laCoordenadaY)){
	    direccion = 'I';
	    sentido   = 'D';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;c<=columna;f++,c++,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];
		}
	}

	//Vertical Directo
	if ((fila>laCoordenadaX) && (columna==laCoordenadaY)){
	    direccion = 'V';
	    sentido   = 'D';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;f<=fila;f++,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];
		}
	}

	//Derecha Indirecto
	if ((fila>laCoordenadaX) && (columna<laCoordenadaY)){
	    direccion = 'D';
	    sentido   = 'I';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;c>=columna;f++,c--,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];
		}
	}

	//Horizontal Indirecto
	if ((fila==laCoordenadaX) && (columna<laCoordenadaY)){
	    direccion = 'H';
	    sentido   = 'I';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;c>=columna;c--,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];
		}
	}

	//Izquierda Indirecto
	if ((fila<laCoordenadaX) && (columna<laCoordenadaY)){
	    direccion = 'I';
	    sentido   = 'I';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;c>=columna;f--,c--,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];
		}
	}

	//Vertical Indirecto
	if ((fila<laCoordenadaX) && (columna==laCoordenadaY)){
	    direccion = 'V';
	    sentido   = 'I';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;f>=fila;f--,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];
		}
	}

	//Derecha Directo
	if ((fila<laCoordenadaX) && (columna>laCoordenadaY)){
	    direccion = 'D';
	    sentido   = 'D';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;c<=columna;c++,f--,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];

		}
	}

	//Horizontal Directo
	if ((fila==laCoordenadaX) && (columna>laCoordenadaY)){
	    direccion = 'H';
	    sentido   = 'D';
        for (var f=laCoordenadaX, c=laCoordenadaY, z=0;c<=columna;c++,z++){
			eval("letra"+numSeccion+"_"+f+"_"+c+".style.background = '"+colorActual+"';");
			valor+=eval("sopa"+numSeccion)[f][c];

		}
	}

	estadoSopa = 0;
	colorActual = generaColor();
	var campoFormulario = "palabra"+numSeccion+"_"+numLetra;
	//setValue(nombreFormulario,campoFormulario,valor);
	
	campoFormulario = "coordenadas"+numSeccion+"_"+numLetra;
	//var valor = getValue(nombreFormulario,campoFormulario);
	//valor = implode(";",new Array(valor,implode(",",new Array(fila,columna))));
	//setValue(nombreFormulario,campoFormulario,valor);

   }// fin else


}//marcarSopa




















function borrar(filas, columnas){

	for ( i = 0; i < filas ; i++){
		for ( j = 0; j < columnas ; j++){
			eval("letra"+"_"+i+"_"+j+".style.background = '#FFFFFF';");
	 	}
	}
	
}//borrar

















