# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)

User.create(username: 'user1', hashed_password: '9fffe4b48bdd5f364339cf26932ae00325190f42', salt: "1337", bitbars: 200) #password = one
User.create(username: 'user2', hashed_password: 'c492e1d6a3533e6ba8ad01647119182b7de320c5', salt: "1337", bitbars: 200) #password = two
User.create(username: 'user3', hashed_password: '73c44f29baf4fc9e56593c01e707f0c48e85e61b', salt: "217703101022879492631352681842557793628", bitbars: 100000) #password = password
User.create(username: 'attacker', hashed_password: 'f05f93ed519d62345facf4f441c7eb32759817ce', salt: "21834708492970860368940710131560218741", bitbars: 0) #password = attacker
