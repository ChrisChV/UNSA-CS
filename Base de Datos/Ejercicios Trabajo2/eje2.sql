create database tienda;
use tienda;

create table Clientes(
DNI char(8) not null,
apellidos varchar(100) not null,
nombres varchar (100) not null,
dir varchar(200) not null,
fechanac date not null);

describe Clientes;

alter table Clientes
add constraint pk_Clientes
primary key(DNI);

create table Producto(
codPro int not null,
nombre varchar(100) not null,
RUC char(11) not null);

describe Producto;

alter table Producto
add constraint pk_Producto
primary key(codPro);

create table Compras(
codCompras int not null,
DNI char(8) not null,
codPro int not null);

alter table Compras
add constraint pk_compras
primary key(codCompras);

alter table Compras
add constraint fk_compras_cliente
foreign key(DNI)
references Clientes(DNI);

alter table Compras
add constraint fk_compras_producto
foreign key(codPro)
references Producto(codPro);

create table Proveedor(
RUC char(11) not null,
nombre varchar(100) not null,
dir varchar(200));

describe Proveedor;

alter table Proveedor
add constraint pk_proveedor
primary key(RUC);

alter table Producto
add constraint fk_producto_proveedor
foreign key(RUC)
references Proveedor(RUC);

