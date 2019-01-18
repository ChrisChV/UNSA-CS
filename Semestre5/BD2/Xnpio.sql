drop view productos;
create view
productos as
select distinct Producto from Ventas;

drop view days;
create view
days as
select * from Dias;

drop view productos_dias;
create view
productos_dias as
select * from (productos cross join days);

drop view productos_montos;
create view
productos_montos as
select Producto, Fecha, sum(Monto) as s from Ventas
group by Producto,Fecha;

select productos_dias.Producto,  productos_dias.Fecha, If(isnull(productos_montos.s),0,productos_montos.s) as monto
	from productos_dias left join productos_montos on productos_dias.Producto = productos_montos.Producto
		and productos_dias.Fecha = productos_montos.Fecha
			order by productos_dias.Producto, productos_dias.Fecha asc;