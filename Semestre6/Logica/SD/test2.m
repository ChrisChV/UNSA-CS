pkg load fuzzy-logic-toolkit
a = newfis('CD Entrenaminto','sugeno','algebraic_product','algebraic_sum','min','max','wtaver');
a = addvar(a,'input','Numero de Repeticiones',[0,25]);
a = addmf(a,'input',1,'Muy Pocas','trapmf',[-5 -2 3 6]);
a = addmf(a,'input',1,'Pocas','trimf',[3 6 12]);
a = addmf(a,'input',1,'Normal','trimf',[6 12 17]);
a = addmf(a,'input',1,'Muchas','trimf',[12 17 20]);
a = addmf(a,'input',1,'Demasiadas','trapmf',[17 20 25 30]);

a = addvar(a,'input','Peso',[0,100]);
a = addmf(a,'input',2,'Muy Bajo','trapmf',[-5 -2 20 40]);
a = addmf(a,'input',2,'Poca','trimf',[20 40 60]);
a = addmf(a,'input',2,'Algo','trimf',[40 60 70]);
a = addmf(a,'input',2,'Mucha','trimf',[60 70 80]);
a = addmf(a,'input',2,'Muchisima','trapmf',[70 80 100 110]);

%a = addvar(a,'output','Tipo de Programador Competitivo',[-25,25]);
%a = addmf(a,'output',1,'Dismuniur', 'constant',[-15]);
%a = addmf(a,'output',1,'Mantener', 'constant',[0]);
%a = addmf(a,'output',1,'Aumentar', 'constant',[15]);


%a = addrule(a, [1 1 1 1 1; 1 2 1 0.5 1; 1 3 2 1 1; 1 4 2 0.5 1; 1 5 3 0.5 1; ...
%				2 1 1 0.5 1; 2 2 2 1 1; 2 3 2 0.5 1; 2 4 3 0.5 1; 2 5 3 1 1; ...
%				3 1 2 1 1; 3 2 3 0.5 1; 3 3 3 1 1; 3 4 4 0.5 1; 3 5 4 1 1; ...
%				4 1 2 1 1; 4 2 3 0.5 1; 4 3 4 0.5 1; 4 4 4 1 1; 4 5 5 0.5 1; ...
%				5 1 3 0.5 1; 5 2 3 1 1; 5 3 4 1 1; 5 4 5 0.5 1; 5 5 5 1 1]);

plotmf(a,'input',1);
plotmf(a,'input',2);

%gensurf(a)


