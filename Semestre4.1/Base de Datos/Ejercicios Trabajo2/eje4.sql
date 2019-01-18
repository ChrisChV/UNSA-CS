create database ejercicio4;
use ejercicio4;


create table Coche(
Matricula char(11) not null,
Marca varchar(100) not null,
Precio int not null,
Color varchar(100),
CodEmpresa int not null,
Modelo varchar(100));

alter table Coche
add constraint pk_coche
primary key(Matricula);

describe Coche;

create table Revision(
CodRev int not null,
Matricula char(11) not null);

alter table Revision
add constraint pk_revision
primary key(CodRev);

create table Cliente(
CodEmpresa int not null,
DNI char(8) not null,
Nombre varchar(100) not null,
Dir varchar(200),
Ciudad varchar(100),
Telefono char(8));

alter table Cliente
add constraint pk_Clientes
primary key(CodEmpresa);

create table Partes(
CodParte int not null,
Descripcion varchar(200) not null);

alter table Partes
add constraint pk_Partes
primary key(CodParte);

create table Partes_Revision(
CodPR int not null,
CodParte int not null,
CodRev int not null);

alter table Partes_Revision
add constraint pk_PR
primary key(CodPR);

alter table Coche
add constraint fk_Coche_Cliente
foreign key(CodEmpresa)
references Cliente(CodEmpresa);

alter table Revision
add constraint fk_Revision_Coche
foreign key(Matricula)
references Coche(Matricula);

alter table Partes_Revision
add constraint fk_PR_Revision
foreign key(CodRev)
references Revision(CodRev);

alter table Partes_Revision
add constraint fk_PR_Partes
foreign key(CodParte) 
references Partes(CodParte);
