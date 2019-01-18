class CreateUsers < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.string :username
      t.string :hashed_password
      t.string :salt
      t.string :profile
      t.integer :bitbars
      t.timestamps
    end
  end
end
