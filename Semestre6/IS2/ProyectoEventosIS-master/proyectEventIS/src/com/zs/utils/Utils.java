package com.zs.utils;

import java.util.ArrayList;

public class Utils {
	
	public Utils() {
		super();
		// TODO Auto-generated constructor stub
	}

	public ArrayList<String> getCountries(){
		ArrayList<String> countries = new ArrayList<String>();
		countries.add("Argentina");
		countries.add("Bolivia");
		countries.add("Brasil");
		countries.add("Colombia");
		countries.add("Chile");
		countries.add("Ecuador");
		countries.add("Paraguay");
		countries.add("Peru");
		countries.add("Uruguay");
		countries.add("Venezuela");
	    return countries;
	}
	
	public ArrayList<String> getCategories(){
		ArrayList<String> categories = new ArrayList<String>();
		categories.add("Autor");
		categories.add("Profesional");
		categories.add("Estudiante");
		categories.add("Empresa");
		categories.add("Libre");
	    return categories;
	}
}
