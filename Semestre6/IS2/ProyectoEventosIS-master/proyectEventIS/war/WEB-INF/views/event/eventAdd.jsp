<%@ page import="com.zs.utils.Utils" %>
<%@ page import="java.util.ArrayList" %>
<html>
<body>
	<h1>Registrar Evento</h1>
	
	Ir a : <a href="list">Lista de Eventos</a>
	<hr />
	
	<%
		Utils u = new Utils();
		ArrayList<String> countries = u.getCountries();
		ArrayList<String> categories = u.getCategories();
	%>

	<form method="post" action="add">
		<table>
			<tr>
				<td>Nombre :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="firstName" id="firstName" /></span></td>
			</tr>
			<tr>
				<td>Apellidos :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="lastName" id="lastName" /></span></td>
			</tr>
			<tr>
				<td>Institución :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="institution" id="institution" /></span></td>
			</tr>
			<tr>
				<td>País :</td>
	    		<td>
		    		<select name="country" id="country">
		    			<% for(int i = 0; i < countries.size(); i++){ %>
          					<option value="<%=countries.get(i)%>"><%=countries.get(i)%></option>
                    	<% } %>
		    		</select>
	    		</td>
	    	</tr>
	    	<tr>
	    		<td>Teléfono :</td>
	    		<td><input type="text" style="width: 185px;" maxlength="30"
	    			name="phone" id="phone" /></span></td>
	    	</tr>
			<tr>
				<td>Email :</td>
				<td><input type="text" style="width: 185px;" maxlength="30"
					name="email" id="email" /></span></td>
			</tr>
			<tr>
				<td>Categoría :</td>
				<td>
					<input type="radio" name="category" value="<%=categories.get(0)%>" checked> <%=categories.get(0)%><br>
					<% for(int i = 1; i < categories.size(); i++){ %>
          				<input type="radio" name="category" value="<%=categories.get(i)%>"><%=categories.get(i)%></br>
                    <% } %>
				</td>
			</tr>
		</table>
		<input type="submit" class="save" title="Save" value="Save" />
	</form>

</body>
</html>