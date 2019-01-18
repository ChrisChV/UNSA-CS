create database ejercicio5;
use ejercicio5;

create table Paciente(
CodPaciente int not null,
Nombre varchar(100) not null,
Apellido varchar(100) not null,
Dir varchar(200),
Ciudad varchar(100),
Distrito varchar(100),
Telefono char(8),
FechaNac date);

alter table Paciente
add constraint pk_Paciente
primary key(CodPaciente);

describe Paciente;

create table Ingreso(
CodIngreso int not null,
Cama varchar(20) not null,
Habitacion varchar(20) not null,
Fecha date not null,
CodPaciente int not null,
CodMedico int not null);

alter table Ingreso
add constraint pk_Ingreso
primary key(CodIngreso);

describe Ingreso;

create table Medico(
CodMedico int not null,
Nombre varchar(100) not null,
Apellidos varchar(100) not null,
Especialidad varchar(100) not null,
Telefono char(8));

alter table Medico
add constraint pk_Medico
primary key(CodMedico);

describe Medico;

alter table Ingreso
add constraint fk_Ingreso_Medico
foreign key(CodMedico)
references Medico(CodMedico);

alter table Ingreso
add constraint fk_Ingreso_Paciente
foreign key(CodPaciente)
references Paciente(CodPaciente);