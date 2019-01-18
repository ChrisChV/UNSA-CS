# Be sure to restart your server when you modify this file.

# This disables HTTP-only cookies. You may not change this setting. 
Bitbar::Application.config.session_store :cookie_store, key: '_bitbar_session', :httponly => false
