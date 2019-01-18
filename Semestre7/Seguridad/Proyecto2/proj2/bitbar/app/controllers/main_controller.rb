class MainController < ApplicationController
  
  def index
    if logged_in? 
      render :index_logged_in
    else
      render :index_logged_out
    end
  end
end
