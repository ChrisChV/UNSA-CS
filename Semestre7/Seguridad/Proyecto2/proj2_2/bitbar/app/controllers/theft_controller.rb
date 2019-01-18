class TheftController < ApplicationController
  # This controller is the destination of the cookies and login credentials you will be stealing 
  # in attacks a and e. It stores the stolen values in your browser's session, and then lets you 
  # view them later. Of course, in a real system/attack you would send this information somewhere 
  # else, but putting it here makes the project simpler.
  #
  # You should not modify these functions or their associated views in any way. 
  def steal_cookie
    session[:stolen_cookie] = params[:cookie][0..63]
    # Only store the first 64 characters to prevent the cookie from growing when stolen multiple times
    head :ok
  end
  
  def view_stolen_cookie
    @cookie = session[:stolen_cookie]
    render :view_stolen_cookie
  end
  
  def steal_login
    session[:stolen_username] = params[:username]
    session[:stolen_password] = params[:password]
    head :ok
  end
  
  def view_stolen_login
    @username = session[:stolen_username]
    @password = session[:stolen_password]
    render :view_stolen_login
  end

  def view_users
    @users = User.all
    render :view_users
  end
end
