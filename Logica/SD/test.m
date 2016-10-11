pkg load fuzzy-logic-toolkit
a = newfis('Tipo de Jugador','sugeno','algebraic_product','algebraic_sum','min','max','wtaver');
a = addvar(a,'input','Altura',[0,300]);
a = addmf(a,'input',1,'Muy Pequeño','trapmf',[-1 0 40 90]);
a = addmf(a,'input',1,'Pequeño','trapmf',[40 90 120 150]);
a = addmf(a,'input',1,'Mediano','trapmf',[120 150 170 180]);
a = addmf(a,'input',1,'Grande','trapmf',[170 190 210 220]);
a = addmf(a,'input',1,'Muy Grande','trapmf',[210 230 300 301]);

a = addvar(a,'input','Habilidad',[0,300]);
a = addmf(a,'input',2,'Muy Poca','trapmf',[-1 0 40 90]);
a = addmf(a,'input',2,'Poca','trapmf',[40 90 120 150]);
a = addmf(a,'input',2,'Algo','trapmf',[120 150 170 180]);
a = addmf(a,'input',2,'Mucha','trapmf',[170 190 210 220]);
a = addmf(a,'input',2,'Muchisima','trapmf',[210 230 300 301]);

a = addvar(a,'output','Tipo de Jugador',[0,20]);
a = addmf(a,'output',1,'Muy Malo', 'constant',[3]);
a = addmf(a,'output',1,'Malo', 'constant',[7]);
a = addmf(a,'output',1,'Regular', 'constant',[13]);
a = addmf(a,'output',1,'Bueno', 'constant',[16]);
a = addmf(a,'output',1,'Muy Bueno', 'constant',[20]);

a = addrule(a, [1 1 1 1 1; 1 2 1 0.5 1; 1 3 2 1 1; 1 4 2 0.5 1; 1 5 3 0.5 1; ...
				2 1 1 0.5 1; 2 2 2 1 1; 2 3 2 0.5 1; 2 4 3 0.5 1; 2 5 3 1 1; ...
				3 1 2 1 1; 3 2 3 0.5 1; 3 3 3 1 1; 3 4 4 0.5 1; 3 5 4 1 1; ...
				4 1 2 1 1; 4 2 3 0.5 1; 4 3 4 0.5 1; 4 4 4 1 1; 4 5 5 0.5 1; ...
				5 1 3 0.5 1; 5 2 3 1 1; 5 3 4 1 1; 5 4 5 0.5 1; 5 5 5 1 1]);

plotmf(a,'output',1);
%showrule(a);
gensurf(a)

