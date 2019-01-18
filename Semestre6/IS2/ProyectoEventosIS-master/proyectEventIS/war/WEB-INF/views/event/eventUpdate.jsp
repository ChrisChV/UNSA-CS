<%@ page import="com.zs.models.Event" %>
<%@ page import="com.zs.utils.Utils" %>
<%@ page import="java.util.ArrayList" %>
<%@ page import="com.google.appengine.api.datastore.KeyFactory" %>
<html>
<body>
	<h1>Editar Evento</h1>
	
	<%
		Event event = new Event();
	
		if(request.getAttribute("event")!=null){
		
			event = (Event)request.getAttribute("event");
			
		}
		
		Utils u = new Utils();
		ArrayList<String> countries = u.getCountries();
		ArrayList<String> categories = u.getCategories();
		
	%>
	
	<form method="post" action="../update" >
		<input type="hidden" name="id" id="id" 
			value="<%=KeyFactory.keyToString(event.getId()) %>" /> 
		
		<table>
			<tr>
				<td>Nombre :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="firstName" id="firstName" value="<%=event.getFirstName() %>" /></td>
			</tr>
			<tr>
				<td>Apellidos :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="lastName" id="lastName" value="<%=event.getLastName() %>" /></td>
			</tr>
			<tr>
				<td>Institución :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="institution" id="institution" value="<%=event.getInstitution() %>" /></td>
			</tr>
			<tr>
				<td>País :</td>
	    		<td>
		    		<select name="country" id="country">
		    			<% for(int i = 0; i < countries.size(); i++){ %>
          					<option value="<%=countries.get(i)%>" <%if(countries.get(i).toString().equals(event.getCountry())){%> selected <%}%>><%=countries.get(i)%></option>
                    	<% } %>
		    		</select>
	    		</td>
	    	</tr>
	    	<tr>
	    		<td>Teléfono :</td>
	    		<td><input type="text" style="width: 185px;" maxlength="30"
	    			name="phone" id="phone" value="<%=event.getPhone() %>" /></td>
	    	</tr>
			<tr>
				<td>Email :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="email" id="email" value="<%=event.getEmail() %>" /></td>
			</tr>
			<tr>
				<td>Categoría :</td>
				<td>
					<%for(int i = 0; i < categories.size(); i++){ 
						if(categories.get(i).toString().equals(event.getCategory())){%>
          					<input type="radio" name="category" value="<%=categories.get(i)%>" checked="checked"><%=categories.get(i)%></br>
                    <% 	}else{ %>
                    		<input type="radio" name="category" value="<%=categories.get(i)%>"><%=categories.get(i)%></br>
                    <% 	} }%>
				</td>
			</tr>
		</table>
		<input type="submit" class="update" title="Update" value="Update" />
	</form>
	
</body>
</html>