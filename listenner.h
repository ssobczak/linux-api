/*
 *  Created on: Feb 22, 2013
 *      Author: ssobczak
 */

#ifndef LISTENNER_H_
#define LISTENNER_H_

#include <list>
#include <map>

#include "listenner.h"

template<class EVT, class DATA>
class Listenner {
public:
	Listenner() {}
	virtual ~Listenner() {}

	virtual void register_event(const EVT& event, const DATA& data);
};

template<class EVT, class DATA>
class Notifier {
public:
	Notifier();
	virtual ~Notifier();

	void add_listenner(const EVT& event, Listenner<EVT, DATA>* listenner);
	bool remove_listenner(const EVT& event, Listenner<EVT, DATA>* listenner);

	void notify(const EVT& event, const DATA& data);

private:
	typedef std::list<Listenner<EVT, DATA>* > ListennersList;
	typedef std::map<EVT, ListennersList > ListennersMap;

	ListennersMap l_map;
};

template<class EVT, class DATA>
void Notifier<EVT, DATA>::add_listenner(const EVT& event, Listenner<EVT, DATA> *listenner) {
	l_map[event].push_back(listenner);
}

template<class EVT, class DATA>
bool Notifier<EVT, DATA>::remove_listenner(const EVT& event, Listenner<EVT, DATA> *listenner) {
	typename ListennersMap::iterator m_it = l_map.find(event);
	if (m_it == l_map.end()) {
		return false;
	}

	typename ListennersList::iterator l_it = m_it->second.find(listenner);
	if (l_it != m_it->end()) {
		return false;
	}

	m_it->second.erase(l_it);
	if (m_it.second.empty()) {
		l_map.erase(m_it);
	}

	return true;
}

template<class EVT, class DATA>
void Notifier<EVT, DATA>::notify(const EVT& event, const DATA& data) {
	typename ListennersMap::iterator m_it = l_map.find(event);
	if (m_it == l_map.end()) {
		return;
	}

	for (typename ListennersList::iterator l_it = m_it->second.begin(); l_it != m_it->second.end(); l_it++) {
		(*l_it)->register_event(event, data);
	}
}

#endif /* LISTENNER_H_ */
