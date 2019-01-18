create database ejercicio2;
use ejercicio2;

create table Camiones(
Matricula char(11) not null,
Modelo varchar(100) not null,
Tipo varchar(100) not null,
Potencia varchar(100) not null);

alter table Camiones
add constraint pk_camiones
primary key(Matricula);

describe Camiones;

create table Camiones_Camioneros(
Cod int not null,
Matricula char(11) not null,
DNI char(8) not null);

alter table Camiones_Camioneros
add constraint pk_Camiones_Camioneros
primary key(Cod);

describe Camiones_Camioneros;

create table Camioneros(
DNI char(8) not null,
Ciudad varchar(100),
Nombre varchar(100) not null,
Telefono varchar(20),
Direccion varchar(200),
Salario int not null);

alter table Camioneros
add constraint pk_Camioneros
primary key(DNI);

describe Camioneros;

alter table Camiones_Camioneros
add constraint fk_Camiones_CC
foreign key(Matricula)
references Camiones(Matricula);

alter table Camiones_Camioneros
add constraint fk_Camioneros_CC
foreign key(DNI) 
references Camioneros(DNI);

create table Paquetes(
CodPaq int not null,
Descripcion varchar(200) not null,
Destinatario varchar(100) not null,
Dir varchar(200) not null,
DNI char(8) not null,
CodProv int not null);


alter table Paquetes
add constraint pk_paquetes
primary key(CodPaq);

describe Paquetes;

create table Provincia(
CodProv int not null,
Nombre varchar(100));

alter table Provincia
add constraint pk_provincia
primary key(CodProv);

describe Provincia;

alter table Paquetes
add constraint fk_paquetes_provincia
foreign key(CodProv)
references Provincia(CodProv);

alter table Paquetes
add constraint fk_paquetes_camioneros
foreign key(DNI)
references Camioneros(DNI);




