module ApplicationHelper
  
  def display_error(error_msg)
    if not error_msg or error_msg == ""
      return ""
    else 
      "<p class='error'>#{error_msg}</p>".html_safe
    end
  end
  
  def sanitize_profile(profile)
    return sanitize(profile, tags: %w(a br b h1 h2 h3 h4 i img li ol p strong table tr td th u ul em span), attributes: %w(id class href colspan rowspan src align valign))
  end
end
