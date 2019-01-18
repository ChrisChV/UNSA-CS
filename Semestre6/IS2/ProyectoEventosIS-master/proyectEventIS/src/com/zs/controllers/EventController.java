package com.zs.controllers;

import java.util.Date;
import java.util.List;

import javax.jdo.PersistenceManager;
import javax.jdo.Query;
import javax.servlet.http.HttpServletRequest;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import com.zs.singleton.PersistenceManagerFact;
import com.zs.models.Event;

@Controller
@RequestMapping("/event")
public class EventController {
	
	@RequestMapping(value = "/add", method = RequestMethod.GET)
	public String getAddEventPage(ModelMap model) {

		return "event/eventAdd";

	}
	
	@RequestMapping(value = "/add", method = RequestMethod.POST)
	public ModelAndView add(HttpServletRequest request, ModelMap model) {

		String firstName = request.getParameter("firstName");
		String lastName = request.getParameter("lastName");
		String institution = request.getParameter("institution");
		String country = request.getParameter("country");
		String phone = request.getParameter("phone");
		String email = request.getParameter("email");
		String category = request.getParameter("category");

		Event e = new Event();
		e.setFirstName(firstName);
		e.setLastName(lastName);
		e.setInstitution(institution);
		e.setCountry(country);
		e.setPhone(phone);
		e.setEmail(email);
		e.setCategory(category);
		e.setDate(new Date());

		PersistenceManager pm = PersistenceManagerFact.get().getPersistenceManager();
		try {
			pm.makePersistent(e);
		} finally {
			pm.close();
		}

		return new ModelAndView("redirect:list");

	}
	
	@RequestMapping(value = "/update/{id}", method = RequestMethod.GET)
	public String getUpdateEventPage(@PathVariable String id,
			HttpServletRequest request, ModelMap model) {

		PersistenceManager pm = PersistenceManagerFact.get().getPersistenceManager();
		
		/*
		Query q = pm.newQuery(Event.class);
		q.setFilter("firstName == nameParameter");
		q.setOrdering("date desc");
		q.declareParameters("String nameParameter");
		*/
		
		try {
			Event e = pm.getObjectById(Event.class, id);
			//List<Event> results = (List<Event>) q.execute(firstName);

			if (e == null) {
				model.addAttribute("event", null);
			} else {
				model.addAttribute("event", e);
			}
		} finally {
			pm.close();
		}

		return "event/eventUpdate";

	}

	@RequestMapping(value = "/update", method = RequestMethod.POST)
	public ModelAndView update(HttpServletRequest request, ModelMap model) {

		String firstName = request.getParameter("firstName");
		String lastName = request.getParameter("lastName");
		String institution = request.getParameter("institution");
		String country = request.getParameter("country");
		String phone = request.getParameter("phone");
		String email = request.getParameter("email");
		String category = request.getParameter("category");
		String id = request.getParameter("id");

		PersistenceManager pm = PersistenceManagerFact.get().getPersistenceManager();

		try {

			Event e = pm.getObjectById(Event.class, id);

			e.setFirstName(firstName);
			e.setLastName(lastName);
			e.setInstitution(institution);
			e.setCountry(country);
			e.setPhone(phone);
			e.setEmail(email);
			e.setCategory(category);
			e.setDate(new Date());

		} finally {

			pm.close();
		}

		// return to list
		return new ModelAndView("redirect:list");

	}

	@RequestMapping(value = "/delete/{id}", method = RequestMethod.GET)
	public ModelAndView delete(@PathVariable String id,
			HttpServletRequest request, ModelMap model) {

		PersistenceManager pm = PersistenceManagerFact.get().getPersistenceManager();

		try {

			Event e = pm.getObjectById(Event.class, id);
			pm.deletePersistent(e);

		} finally {
			pm.close();
		}

		// return to list
		return new ModelAndView("redirect:../list");

	}
	
	// get all customers
	@RequestMapping(value = "/list", method = RequestMethod.GET)
	public String listEvent(ModelMap model) {

		PersistenceManager pm = PersistenceManagerFact.get().getPersistenceManager();
		Query q = pm.newQuery(Event.class);
		q.setOrdering("date desc");

		List<Event> results = null;

		try {
			results = (List<Event>) q.execute();

			if (results.isEmpty()) {
				model.addAttribute("eventList", null);
			} else {
				model.addAttribute("eventList", results);
			}

		} finally {
			q.closeAll();
			pm.close();
		}

		return "event/eventList";

	}
	
}
