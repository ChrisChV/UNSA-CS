Warnup

Se modifica la función view_profile del archivo user_controller.rb. Se elimina del input los patrones sospechosos: <script>, GET, etc.

A


B

Se agrega un nuevo token a la session que se genera cada vez que se quiera hacer una transacción en user_controller.rb. El toquen se oculta
en la pagina transfer_form.html.erb y luego se lo compara con el que esta guardado en la session.

B+

Igual que el B

C

se cambia el archivo user_controller.rb. Se borra al usuario por la id ya no por el user name.

D

Igual que el Warnup

