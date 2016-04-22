select pizzeria from Frequents inner join Person on Frequents.name = Person.name
	where age < 18;
    

select distinct Person.name from Person inner join Eats on Person.name = Eats.name
	where (pizza = 'mushroom' or pizza = 'pepperoni') 
		and gender = 'female';

        
select Person.name from Person inner join Eats on Person.name = Eats.name
	where gender = 'female' and (pizza = 'mushroom' or pizza = 'pepperoni')
    group by name
    having count(*) = 2;

    
select Serves.pizzeria from Serves inner join Frequents on Serves.pizzeria = Frequents.pizzeria
	where price < 10 and name = 'Amy';
 

select distinct pizzeria from Frequents inner join Person on Frequents.name = Person.name    
	where gender = 'female'
    and pizzeria not in
    (select distinct pizzeria from Frequents inner join Person on Frequents.name = Person.name    
	where gender = 'male')
union
select distinct pizzeria from Frequents inner join Person on Frequents.name = Person.name    
	where gender = 'male'
    and pizzeria not in
    (select distinct pizzeria from Frequents inner join Person on Frequents.name = Person.name    
	where gender = 'female');

    
select name, pizza AS T from Eats
	where (name,pizza) not in
    (select name, Serves.pizza  from Serves inner join Frequents on Serves.pizzeria = Frequents.pizzeria);


select name from Person where name not in
 (select name from Frequents where (name,pizzeria) not in
	(select distinct name, pizzeria from Eats inner join Serves on Eats.pizza = Serves.pizza)); 


select name from Person where name not in
  (select distinct name from Eats inner join Serves on Eats.pizza = Serves.pizza where (name,pizzeria) not in
	(select name,pizzeria from Frequents));


select pizzeria from Serves where pizza = 'pepperoni' and price =
	(select min(price) from Serves where pizza = 'pepperoni');


