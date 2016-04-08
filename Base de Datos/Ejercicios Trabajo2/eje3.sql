create database ejercicio3;
use ejercicio3;

create table Profesores(
DNI char(8) not null,
Nombre varchar(100) not null,
Direccion varchar(200),
Telefono varchar(20));

alter table Profesores
add constraint pk_profesores
primary key(DNI);

describe Profesores;

create table Modulos(
CodMod int not null,
Nombre varchar(100) not null,
DNI char(8) not null);

alter table Modulos
add constraint pk_modulos
primary key(CodMod);

describe Modulos;

create table Alumno(
Expediente int not null,
Nombre varchar(100) not null,
FechaNac date,
Apellidos varchar(100) not null);

alter table Alumno
add constraint pk_alumno
primary key(Expediente);

describe Alumno;

create table Alumno_Modulo(
Cod int not null,
Expediente int not null,
CodMod int not null,
Delegado boolean not null);

alter table Alumno_Modulo
add constraint pk_AM
primary key(Cod);

alter table Alumno_Modulo
add constraint fk_AM_Modulos
foreign key(CodMod)
references Modulos(CodMod);

alter table Alumno_Modulo
add constraint fk_AM_Alumno
foreign key(Expediente)
references Alumno(Expediente);

alter table Modulos
add constraint fk_Modulos_Profesores
foreign key(DNI)
references Profesores(DNI);
