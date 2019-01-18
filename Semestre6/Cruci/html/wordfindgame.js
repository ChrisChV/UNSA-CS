/**
* Wordfind.js 0.0.1
* (c) 2012 Bill, BunKat LLC.
* Wordfind is freely distributable under the MIT license.
* For all details and documentation:
*     http://github.com/bunkat/wordfind
*/

(function (document, $, wordfind) {

  'use strict';

  /**
  * An example game using the puzzles created from wordfind.js. Click and drag
  * to highlight words.
  *
  * WordFindGame requires wordfind.js and jQuery.
  */

  /**
  * Initializes the WordFindGame object.
  *
  * @api private
  */
  var WordFindGame = function() {

    // List of words for this game
    var wordList;

    /**
    * Draws the puzzle by inserting rows of buttons into el.
    *
    * @param {String} el: The jQuery element to write the puzzle to
    * @param {[[String]]} puzzle: The puzzle to draw
    */
    var drawPuzzle = function (el, puzzle) {
      
      var output = '';
      // for each row in the puzzle
      for (var i = 0, height = puzzle.length; i < height; i++) {
        // append a div to represent a row in the puzzle
        var row = puzzle[i];
        output += '<div>';
        // for each element in that row
        for (var j = 0, width = row.length; j < width; j++) {
            // append our button with the appropriate class
            output += '<button class="puzzleSquare" x="' + j + '" y="' + i + '">';
            output += row[j] || '&nbsp;';
            output += '</button>';
        }
        // close our div that represents a row
        output += '</div>';
      }

      $(el).html(output);
    };

    /**
    * Draws the words by inserting an unordered list into el.
    *
    * @param {String} el: The jQuery element to write the words to
    * @param {[String]} words: The words to draw
    */
    var drawWords = function (el, words, words1) {
      
      var tips = [];
      tips[0] = '(Acrilonitrilo-butadieno-estireno). Los productos de esta familia son muy tenaces, no quebradizos, duros, rígidos y resistentes a los productos químicos. Son un terpolímero de tres monómeros, frecuentemente considerado como un poliestireno modificado puesto que sus propiedades son parecidas al PS, excepto que su resistencia al impacto es muy alta. El ABS típico contiene aproximadamente el 20% caucho, 25% acrilonitrilo, y 55% estireno.';
      tips[1] = 'Un material, p,ej. una carga, añadido a un compuesto plástico, el cual puede alterar las propiedades del producto terminado, pero que no reacciona químicamente con ninguno de los componentes del compuesto.';
      tips[2] = 'Especie de talega o saco de una materia flexible, que sirve para guardar o llevar alguna cosa. Una especie de bolsa plana, de plástico papal, soldada por tres lados y con una dobladura en el lado abierto.';
      tips[3] = 'Una boquilla está diseñada para formar bajo presión un sello entre el cilindro calefactor o la cámara de transferencia y el molde, la cual le dependiendo de la geometría y forma de esta le dara las formas las formas al material extruido.';
      tips[4] = 'Boquilla utilizado para la obtencion de tubos';
      tips[5] = 'Nombre utilizado corrientemente, en lugar de relleno. Material sólido e inerte, que se añade a un plástico para modificar sus propiedades.';
      tips[6] = 'Consiste en cuchillas estacionarias o de tipo guillotina si la velocidad de extrusión es suficientemente baja o el material es flexible , o en cuchillas transportables en el caso de materiales rigidos . cada ves mas frecuente el cortador por chorro de aire.';
      tips[7] = 'Para cambiar las propiedades físicas de un material por reacción química, que puede ser condensación, polimerización o vulcanización; generalmente se realiza por la acción de Calor y catalizadores, solos o en combinación, con o sin presión';
      tips[8] = 'Proceso de compactación y fusión de un material plástico , forzándolo a través de un orificio de manera continua. El material es transportado a través del cilindro de la máquina calentado por un tornillo helicoidal o tornillos, donde es calentado y mezclado a un estado homogéneo y luego forzado a través de una matriz de la forma requerida para el producto acabado.';
      tips[9] = 'Máquina utilizada para recuperar sobrantes, desechos, y piezas rechazadas para reducirlas a partículas, de un tamaño que puedan ser reprocesadas. También se les llama molinos y recicladoras.';
      tips[10] = 'Porción de cualquier materia extendida en superficie y de poco grosor.  Un simple pliegue, doblez o capa en un laminado, el cual está hecho por una superposición de capas.';
      tips[11] = 'Un material plástico en una condición fundida o plastificada.';
      tips[12] = 'Dícese del estado en el cual, las moléculas se hallan ordenadas en formas características, de modo que forman sólidos que presentan diferentes propiedades, según la dirección (anisotropía) y pueden tomar espontáneamente caras planas.';
      tips[13] = 'Es la acción por la cual dos o mas polímeros se van a unir molecularmente , para ello los polímeros se sirven en formas muy diferentes ,que van desde gránulos (granza ), polvos, planchas,resinas liquidas de bajo peso molecular.Se distingue dos tipos de mezclado: Intensivo o disperso(compounding ) y Extensivo o distribuido (blending).';
      tips[14] = '(Poliestireno) Es un homopolímero amorfo del estireno, que se obtiene por polimerización en bloque o en suspensión a través de radicales libres.';
      tips[15] = 'Las películas se obtienen: por la extrusión de un plástico fundido, utilizando una hilera circular o plana, por calandrado, por solución colada, por conversión química, o por pelado. Las películas pueden ser orientadas uniaxial o biaxial; o bien laminadas para modificar sus propiedades. Las películas se distinguen de las hojas en la industria del plástico y de embalaje por su grueso. Una lámina de menos de 0,25 mm. se le llama normalmente película. Sobre este grueso se le llama hoja.';
      tips[16] = 'Es el método más común para la fabricación de películas o films, y en general se utiliza para fabricar bolsas de plásticos termoplásticos.';
      tips[17] = 'Sin forma regular o bien determinada. Cuerpo sólido no cristalino.';
      tips[18] = 'Las poliolefinas son termoplásticos parcialmente cristalinos del grupo de los plásticos estándar. Los representantes más importantes dentro de este grupo son el polietileno (PE) y el polipropileno (PP), que, juntos, representan aproximadamente la mitad de todo el volumen de producción de los plásticos.';
      tips[19] = 'Un cilindro montado sobre cojinetes, y utilizada en funciones tales como: trabajos de formado, aplastamiento, movimiento, estampado o impresión.';
      tips[20] = 'Cualquier proceso de formación de una lámina termoplástica que consiste en calentar la lámina y empujarla hacia abajo sobre una superficie del molde.';
      tips[21] = 'Estructura propia de los tejidos, que se repite periódicamente en su longuitud y anchura. La forman un número determinado de hilos de urdimbre y otro, igual o diferente, de hilos de trama, que se entrecruzan o ligan siguiendo un orden propio de cada caso.';
      tips[22] = 'Depósito de alimentación en el que se carga polvo de moldeo y desde el que cae en una máquina de moldeo o extrusora, a veces a través de un dispositivo de medición.';
      tips[23] = 'O usillo consiste en un cilindro largo rodeado por un filete helicoidal. El tornillo es una de las partes mas importantes ya que contribuye a realizar las funciones de transportar , calentar , fundir y mezclar el material . La estabilidad del proceso y la calidad del <pr></pr>oducto que se obtiene depende en gran medida del diseño del tornillo';
      tips[24] = 'Fricción interna o resistencia al flujo de un líquido; La relación constante de esfuerzo de cizallamiento a la velocidad de un cizallamiento. En los líquidos para los que esta relación es una función de la tensión, la "viscosidad aparente" se define como esta relación.';



      var output = '<ul>';
      for (var i = 0, len = words.length; i < len; i++) {
        var word = words[i];
        var word1 = words1[i];
        if(word == 'Materiales Cristalinos'){
          word1 = 'materialescristalinos';
        }
        if(word == 'Material Fundido'){
          word1 = 'materialfundido';
        }
        var tip = tips[i];
        output += '<li class="word tip ' + word1 + '">' + word + '<span>' + tip + '</span></li>';
      }
      output += '</ul>';

      $(el).html(output);
    };


    /**
    * Game play events.
    *
    * The following events handle the turns, word selection, word finding, and
    * game end.
    *
    */

    // Game state
    var startSquare, selectedSquares = [], curOrientation, curWord = '';

    /**
    * Event that handles mouse down on a new square. Initializes the game state
    * to the letter that was selected.
    *
    */
    var startTurn = function () {
      $(this).addClass('selected');
      startSquare = this;
      selectedSquares.push(this);
      curWord = $(this).text();
    };



    /**
    * Event that handles mouse over on a new square. Ensures that the new square
    * is adjacent to the previous square and the new square is along the path
    * of an actual word.
    *
    */
    var select = function (target) {
      // if the user hasn't started a word yet, just return
      if (!startSquare) {
        return;
      }

      // if the new square is actually the previous square, just return
      var lastSquare = selectedSquares[selectedSquares.length-1];
      if (lastSquare == target) {
        return;
      }

      // see if the user backed up and correct the selectedSquares state if
      // they did
      var backTo;
      for (var i = 0, len = selectedSquares.length; i < len; i++) {
        if (selectedSquares[i] == target) {
          backTo = i+1;
          break;
        }
      }

      while (backTo < selectedSquares.length) {
        $(selectedSquares[selectedSquares.length-1]).removeClass('selected');
        selectedSquares.splice(backTo,1);
        curWord = curWord.substr(0, curWord.length-1);
      }


      // see if this is just a new orientation from the first square
      // this is needed to make selecting diagonal words easier
      var newOrientation = calcOrientation(
          $(startSquare).attr('x')-0,
          $(startSquare).attr('y')-0,
          $(target).attr('x')-0,
          $(target).attr('y')-0
          );

      if (newOrientation) {
        selectedSquares = [startSquare];
        curWord = $(startSquare).text();
        if (lastSquare !== startSquare) {
          $(lastSquare).removeClass('selected');
          lastSquare = startSquare;
        }
        curOrientation = newOrientation;
      }

      // see if the move is along the same orientation as the last move
      var orientation = calcOrientation(
          $(lastSquare).attr('x')-0,
          $(lastSquare).attr('y')-0,
          $(target).attr('x')-0,
          $(target).attr('y')-0
          );

      // if the new square isn't along a valid orientation, just ignore it.
      // this makes selecting diagonal words less frustrating
      if (!orientation) {
        return;
      }

      // finally, if there was no previous orientation or this move is along
      // the same orientation as the last move then play the move
      if (!curOrientation || curOrientation === orientation) {
        curOrientation = orientation;
        playTurn(target);
      }

    };
    
    var touchMove = function(e) {
      var xPos = e.originalEvent.touches[0].pageX;
      var yPos = e.originalEvent.touches[0].pageY;
      var targetElement = document.elementFromPoint(xPos, yPos);
      select(targetElement)
    };
    
    var mouseMove = function() { 
      select(this);
    };

    /**
    * Updates the game state when the previous selection was valid.
    *
    * @param {el} square: The jQuery element that was played
    */
    var playTurn = function (square) {

      // make sure we are still forming a valid word
      for (var i = 0, len = wordList.length; i < len; i++) {
        if (wordList[i].indexOf(curWord + $(square).text()) === 0) {
          $(square).addClass('selected');
          selectedSquares.push(square);
          curWord += $(square).text();
          break;
        }
      }
    };

    /**
    * Event that handles mouse up on a square. Checks to see if a valid word
    * was created and updates the class of the letters and word if it was. Then
    * resets the game state to start a new word.
    *
    */
    var endTurn = function () {

      // see if we formed a valid word
      for (var i = 0, len = wordList.length; i < len; i++) {
        
        if (wordList[i] === curWord) {
          $('.selected').addClass('found');
          wordList.splice(i,1);
          $('.' + curWord).addClass('wordFound');
        }

        if (wordList.length === 0) {
          $('.puzzleSquare').addClass('complete');
        }
      }

      // reset the turn
      $('.selected').removeClass('selected');
      startSquare = null;
      selectedSquares = [];
      curWord = '';
      curOrientation = null;
    };

    /**
    * Given two points, ensure that they are adjacent and determine what
    * orientation the second point is relative to the first
    *
    * @param {int} x1: The x coordinate of the first point
    * @param {int} y1: The y coordinate of the first point
    * @param {int} x2: The x coordinate of the second point
    * @param {int} y2: The y coordinate of the second point
    */
    var calcOrientation = function (x1, y1, x2, y2) {

      for (var orientation in wordfind.orientations) {
        var nextFn = wordfind.orientations[orientation];
        var nextPos = nextFn(x1, y1, 1);

        if (nextPos.x === x2 && nextPos.y === y2) {
          return orientation;
        }
      }

      return null;
    };

    return {

      /**
      * Creates a new word find game and draws the board and words.
      *
      * Returns the puzzle that was created.
      *
      * @param {[String]} words: The words to add to the puzzle
      * @param {String} puzzleEl: Selector to use when inserting the puzzle
      * @param {String} wordsEl: Selector to use when inserting the word list
      * @param {Options} options: WordFind options to use when creating the puzzle
      */
      create: function(words, words2, puzzleEl, wordsEl, options) {
        
        wordList = words;
        var wordList2 = words2.slice(0).sort();

        var puzzle = wordfind.newPuzzle(words, options);

        // draw out all of the words
        drawPuzzle(puzzleEl, puzzle);
        drawWords(wordsEl, wordList2, wordList);

        // attach events to the buttons
        // optimistically add events for windows 8 touch
        if (window.navigator.msPointerEnabled) {
          $('.puzzleSquare').on('MSPointerDown', startTurn);
          $('.puzzleSquare').on('MSPointerOver', select);
          $('.puzzleSquare').on('MSPointerUp', endTurn);
        }
        else {
          $('.puzzleSquare').mousedown(startTurn);
          $('.puzzleSquare').mouseenter(mouseMove);
          $('.puzzleSquare').mouseup(endTurn);
          $('.puzzleSquare').on("touchstart", startTurn);
          $('.puzzleSquare').on("touchmove", touchMove);
          $('.puzzleSquare').on("touchend", endTurn);
        }

        return puzzle;
      },

      /**
      * Solves an existing puzzle.
      *
      * @param {[[String]]} puzzle: The puzzle to solve
      * @param {[String]} words: The words to solve for
      */
      solve: function(puzzle, words) {

        var solution = wordfind.solve(puzzle, words).found;

        for( var i = 0, len = solution.length; i < len; i++) {
          var word = solution[i].word,
              orientation = solution[i].orientation,
              x = solution[i].x,
              y = solution[i].y,
              next = wordfind.orientations[orientation];

          if (!$('.' + word).hasClass('wordFound')) {
            for (var j = 0, size = word.length; j < size; j++) {
              var nextPos = next(x, y, j);
              $('[x="' + nextPos.x + '"][y="' + nextPos.y + '"]').addClass('solved');
            }

            $('.' + word).addClass('wordFound');
          }
        }

      }
    };
  };


  /**
  * Allow game to be used within the browser
  */
  window.wordfindgame = WordFindGame();

}(document, jQuery, wordfind));



