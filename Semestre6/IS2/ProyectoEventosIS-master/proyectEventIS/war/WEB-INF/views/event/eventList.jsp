<%@ page import="java.util.List" %>
<%@ page import="com.zs.models.Event" %>
<%@ page import="com.google.appengine.api.datastore.KeyFactory" %>
<html>
<body>
	<h1>Eventos</h1>

	Ir a : <a href="add">Agregar Evento</a>
	<hr />

	<h2>All Customers</h2>
	<table border="1">
		<thead>
			<tr>
				<td>Nombre</td>
				<td>Apellidos</td>
				<td>Institución</td>
				<td>País</td>
				<td>Teléfono</td>
				<td>Email</td>
				<td>Categoría</td>
				<td>Creado</td>
				<td>Acción</td>
			</tr>
		</thead>
		
		<%
			
			if(request.getAttribute("eventList")!=null){
				
				List<Event> events = (List<Event>)request.getAttribute("eventList");
				
				if(!events.isEmpty()){
					 for(Event e : events){
						 
		%>
					<tr>
					  <td><%=e.getFirstName() %></td>
					  <td><%=e.getLastName() %></td>
					  <td><%=e.getInstitution() %></td>
					  <td><%=e.getCountry() %></td>
					  <td><%=e.getPhone() %></td>
					  <td><%=e.getEmail() %></td>
					  <td><%=e.getCategory() %></td>
					  <td><%=e.getDate() %></td>
					  <td><a href="update/<%=KeyFactory.keyToString(e.getId()) %>">Actualizar</a> 
		                   | <a href="delete/<%=KeyFactory.keyToString(e.getId()) %>">Eliminar</a></td>
					</tr>
		<%
			
					}
		    
				}
			
		   	}
		%>
         
        </tr>
     
	</table>

</body>
</html>