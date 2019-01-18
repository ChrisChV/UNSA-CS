use sesion3;

select * from clientes;

select Nombre from clientes where Poblacion like 'Arequipa';

select * from venta;

select CodProducto, Descripcion from Producto where CodProducto = Descripcion;

select Nombre, IdVenta, Cantidad from clientes join Venta where clientes.NCliente = venta.NCliente and Cantidad >= 500;

select Nombre from Clientes where Nombre not in (select distinct Nombre from Clientes join Venta where Clientes.NCliente = Venta.NCliente);




select IdVenta from Venta where Cantidad > (select Cantidad from Venta where IdVenta like 18);

select IdVenta from Venta where IdVenta not in (select distinct IdVenta from producto join venta join clientes
	where producto.CodProducto = venta.CodProducto
		and clientes.NCliente = venta.NCliente and Poblacion like 'Lima');
        
select IdVenta from Venta where IdVenta not in (select distinct IdVenta from producto join venta join clientes
	where producto.CodProducto = venta.CodProducto
		and clientes.NCliente = venta.NCliente and (Poblacion like 'Lima' or Poblacion = 'Arequipa'));