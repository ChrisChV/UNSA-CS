Bitbar::Application.routes.draw do
  # This file controls how URLs are routed to controllers. 
  # Each rule has the following format:
  # http_method 'url_value' => 'controller#method'
  # 
  # For example:
  #   get 'profile' => 'user#view_profile'
  # routes the GET request to /profile to UserController's view_profile method 
  #
  # For more information, see: 
  # http://guides.rubyonrails.org/routing.html
  
  # Main page
  root 'main#index'
  
  # Accounts
  get 'login' => 'login#login'
  post 'post_login' => 'login#post_login'
  get 'logout' => 'login#logout'
  get 'register' => 'login#register'
  post  'post_register' => 'login#post_register'

  get 'close' => 'user#delete_user'
  post 'close' => 'user#post_delete_user'
  
  # Transfer
  get 'transfer' => 'user#transfer'
  post 'post_transfer' => 'user#post_transfer'
  get 'protected_transfer' => 'user#protected_transfer'
  post 'protected_post_transfer' => 'user#protected_post_transfer'

  get 'super_secure_transfer' => 'user#super_secure_transfer'
  post 'super_secure_post_transfer' => 'user#super_secure_post_transfer'
  
  # Profile
  post 'set_profile' => 'user#set_profile'
  get 'profile' => 'user#view_profile'
  
  # Theft
  # Do not modify these, as they will be used for grading
  get 'steal_cookie' => 'theft#steal_cookie'
  get 'view_stolen_cookie' => 'theft#view_stolen_cookie'
  post 'steal_login' => 'theft#steal_login'
  get 'view_stolen_login' => 'theft#view_stolen_login'
  get 'view_users' => 'theft#view_users'

end
