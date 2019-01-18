class UserController < ApplicationController
  protect_from_forgery :only => [:protected_transfer, :protected_post_transfer]

  def user_params
    params.require(:user).permit(:username, :hashed_password, :salt, :profile, :bitbars)
  end
  
  def transfer
    if not logged_in?
      render "main/must_login"
      return
    end
    
    @user = params[:user]
    @amount = params[:quantity]
    render :transfer_form
  end
  
  def post_transfer(failure_form=:transfer_form)
    if not logged_in?
      render "main/must_login"
      return
    end
    
    destination_username = params[:destination_username]
    @quantity = params[:quantity].to_i
    
    @error = ""
    @source_user = @logged_in_user
    @destination_user = User.find_by_username(destination_username)
    if not @destination_user
      @error = "The recipient does not exist."
    elsif @source_user.bitbars < @quantity
      @error = "You do not have enough bitbars!"
    elsif @destination_user.id == @source_user.id
      @error = "You cannot transfer bitbar to yourself!" 
    end
    
    if @error != ""
      render failure_form
    else
      @source_user.bitbars -= @quantity
      @destination_user.bitbars += @quantity
      @source_user.save
      @destination_user.save
      render :transfer_success
    end 
  end
  
  # Exact same as above, but Rails auto-CSRF protection enabled on it
  def protected_transfer
    transfer
  end
  def protected_post_transfer 
    post_transfer
  end

  # Weak (and ostentatious) CSRF Protection
  def super_secure_transfer
    if not logged_in?
      render "main/must_login"
      return
    end
    @user = params[:user]
    @amount = params[:quantity]
    @token = session[:token]
    render :super_secure_transfer_form
  end

  def super_secure_post_transfer
    if not logged_in?
      render "main/must_login"
      return
    end

    @token = session[:token]
    if params[:tokeninput] != session[:token]
      @error = "Please enter the correct secret token!"
      render :super_secure_transfer_form
      return
    end
    post_transfer :super_secure_transfer_form
  end

  def view_profile
    @username = params[:username]
    @user = User.find_by_username(@username)
    if not @user
      if @username and @username != ""
        @error = "User #{@username} not found"
      elsif logged_in?
        @user = @logged_in_user
      end
    end
    
    render :profile
  end
  
  def set_profile
    if not logged_in?
      render "main/must_login"
      return
    end
    
    @logged_in_user.profile = params[:new_profile]
    @logged_in_user.save
    
    render :set_profile_success
  end

  def delete_user
    if not logged_in?
      render "main/must_login"
      return
    end

    render "user/delete_user"
  end

  def post_delete_user
    if not logged_in?
      render "main/must_login"
      return
    end

    @username = @logged_in_user.username
    User.destroy_all("username = '#{@username}'")

    reset_session
    @logged_in_user = nil
    render "user/delete_user_success"
  end
end
