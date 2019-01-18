package com.zs.singleton;

import javax.jdo.JDOHelper;
import javax.jdo.PersistenceManagerFactory;

public final class PersistenceManagerFact {
	private static final PersistenceManagerFactory pmfInstance = JDOHelper
			.getPersistenceManagerFactory("transactions-optional");

	private PersistenceManagerFact() {
	}

	public static PersistenceManagerFactory get() {
		return pmfInstance;
	}
}