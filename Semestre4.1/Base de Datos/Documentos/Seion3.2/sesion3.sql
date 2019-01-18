create database sesion3;
use sesion3;

create table Clientes(
NCliente int not null,
Nombre varchar(100) not null,
Dir varchar(200),
Telefono char(8),
Poblacion varchar(100) not null);

alter table Clientes
add constraint pk_Clientes
primary key(NCliente);

create table Producto(
CodProducto int not null,
Descripcion varchar(200) not null,
Precio int not null);

alter table Producto
add constraint pk_Producto
primary key(CodProducto);

create table Venta(
IdVenta int not null,
CodProducto int not null,
NCliente int not null,
Cantidad int not null);

alter table Venta
add constraint pk_Venta
primary key(IdVenta);

alter table Venta
add constraint fk_VentaProducto
foreign key(CodProducto)
references Producto(CodProducto);

alter table Venta
add constraint fk_VentaCliente
foreign key(NCliente)
references Clientes(NCliente);