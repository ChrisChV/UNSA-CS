class LoginController < ApplicationController
  
  def generate_random_salt
    return rand(2**128).to_s
  end
  
  def hash_password(password, salt)
    salted_password = "#{salt}#{password}"
    return Digest::SHA1.hexdigest(salted_password)
  end
  
  def login
    render :login_form
  end
  
  def post_login
    @username = params[:username]
    @password = params[:password]
    
    @user = User.find_by_username(@username)
    
    @error = ""
    if @user == nil or hash_password(@password, @user.salt) != @user.hashed_password
      @error = "The login information you entered is not valid."
    end
    
    if @error != ""
      render :login_form
    else
      session[:logged_in_id] = @user.id
      load_logged_in_user
      render :login_success
    end
  end

  def logout
    if @logged_in_user then
        reset_session
    end
    redirect_to(:controller => "main", :action => "index")
  end
  
  def register
    render :register_form
  end
  
  def post_register
    username = params[:username]
    password = params[:password]
    
    @error = ""
    if username == "" or password == ""
      @error = "You must enter a username and password."
    elsif User.find_by_username(username)
      @error = "A user with that name already exists"
    end
    
    if @error != ""
      render :register_form
    elsif
      @user = User.new
      @user.username = username
      @user.salt = generate_random_salt
      @user.hashed_password = hash_password(password, @user.salt)
      @user.profile = ""
      @user.bitbars = 200
      @user.save
      session[:logged_in_id] = @user.id
      load_logged_in_user
      render :register_success
    end
  end
end