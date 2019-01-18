class ApplicationController < ActionController::Base
  before_action :load_logged_in_user
  before_action :disable_protections
  before_action :check_security_settings
  
  # Since this is before_filter in the ApplicationController, this will run for every controller. If the user is logged in, their record will be in the @logged_in_user variable. Otherwise, that variable will be null.
  def load_logged_in_user
    @logged_in_user = User.find_by_id(session[:logged_in_id])
    if not session[:token]
      session[:token] = SecureRandom.urlsafe_base64
    end
  end

  def logged_in?
    return @logged_in_user != nil
  end
  
  def disable_protections
    # This disables some browser built-in XSS protection. For the fixes portion of the lab, these MUST stay disabled. You should fix the vulnerabilities some other way...
    response.headers['X-XSS-Protection'] = '0'
    response.headers.except! 'X-Frame-Options'
  end
  
  def check_security_settings 
    # These settings can be used to selectively disable security measures for a particular part. 
    # You may not use them unless explicitly stated in the problem description, and you may not use
    # them at all for the defenses part of the assignment. 
    @disable_framebusting = params['disable_fb'] == 'yes'
  end
end
