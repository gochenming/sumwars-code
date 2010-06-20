#ifdef SERVERNETWORK_H
#else
#define SERVERNETWORK_H

#include "network.h"
#include <queue>
#include <map>

// Debuggin
#include "debug.h"



/**
 * \class NetworkSlot
 * \brief Slot-Struktur mit Information fuer eine Verbindung
 */
class NetworkSlot
{
	public:
		/**
		 * \fn NetworkSlot(PlayerID client_address,RakServerInterface* peer);
		 * \brief Initialisiert den Slot
		 * \param client_address Netzwerkadresse des angeschlossenen Client
		 * \param peer Interface von RakNet
		 */
		NetworkSlot(PlayerID client_address,RakServerInterface* peer);

		/**
		 * \fn ~NetworkSlot()
		 * \brief Destruktor
		 */
		~NetworkSlot();

		/**
		 * \fn void pushReceivedPacket(Packet* packet)
		 * \brief Fuegt ein empfangenes Paket in die Warteschlange fuer empfangene Pakete ein
		 * \param packet empfangenes Paket
		 */
		void pushReceivedPacket(Packet* packet);

		/**
		 * \fn Packet* popReceivedPacket()
		 * \brief Entnimmt das erste noch nicht abgeholte Paket aus der Warteschlange. Gibt Null zurueck, wenn die Warteschlange leer ist
		 */
		Packet* popReceivedPacket();

		/**
		 * \fn int numberMessages()
		 * \brief Gibt die Anzahl der empfangenen, noch nicht abgeholten Pakete aus
		 */
		int numberMessages();
		
		/**
		 * \fn PlayerID getPlayerID()
		 * \brief Gibt die Adresse des mit diesem Slot verbundenen Client aus
		 */
		PlayerID getPlayerID()
		{
			return m_system_address;
		}

		/**
		 * \fn NetStatus getStatus()
		 * \brief Gibt die Status des Slots aus
		 */
		NetStatus getStatus()
		{
			return m_status;
		}

		/**
		 * \fn void setStatus(NetStatus s)
		 * \brief Setzt den Status des Slots.
		 * \param s neuer Zustand
		 */
		void setStatus(NetStatus s)
		{
			m_status = s;
		}

	private:
		/**
		 * \var PacketQueue m_received_packets
		 * \brief Liste der empfangenen Packete
		 */
		PacketQueue m_received_packets;

		/**
		 * \var PlayerID m_system_address
		 * \brief Adresse des Clients der zu diesem Slot gehoert
		 */
		PlayerID m_system_address;

		/**
		 * \var RakServerInterface* m_peer
		 * \brief Netzwerkschnittstelle von RakNet
		 */
		RakServerInterface* m_peer;

		/**
		 * \var NetStatus m_status
		 * \brief Status des Slots
		 */
		NetStatus m_status;
};

/**
 * \class ServerNetwork
 * \brief Netzwerkschnittstelle auf Serverseite
 */
class ServerNetwork: public Network
{
	public:
		/**
		 * \fn ServerNetwork(int max_slots)
		 * \brief Konstruktor
		 * \param max_slots maximale Anzahl Clients die der Server akzeptiert
		 */
		ServerNetwork(int max_slots);

		/**
		 * \fn ~ServerNetwork()
		 * \brief Destruktor
		 */
		virtual ~ServerNetwork();

		/**
		* \fn NetStatus init( int auth_port )
		* \brief Initialisiert den Server, oeffnet die Netzwerkschnittstelle
		* \param auth_port gibt den Port an, auf dem sich Clients anmelden koennen
		* \return Status der Methode
		*
		*/
		virtual NetStatus init( int auth_port );

		/**
		 * \fn void kill()
		 * \brief Schliesst das die Netzwerkverbindung
		 */
		virtual void kill();

		

		/**
		 * \fn NetStatus getSlotStatus( int slot=0 )
		* \brief Liefert den Status eines Server-Slots
		* \param slot Slotnummer
		* \return Status des Slots
		*
		*/
		NetStatus getSlotStatus( int slot=0 );

		/**
		* \fn NetStatus update()
		* \brief Fuehrt fuer alle Server-Slots die Abarbeitung derer durch
		* \return liefert immer NET_OK
		*/
		void update();


		/**
		 * \fn virtual int numberSlotMessages(int slot=0)
		 * \param slot Nummer des Slots dessen Paketanzahl abgefragt wird
		 * \brief  Gibt Anzahl der Packete im Empfangspuffer aus
		 * \return Anzahl der Packete
		 *
		 * Ist der Slot ungeultig wird Null geliefert, sonst die Anzahl der Packete im Empfangspuffers
	 	 */
		virtual int numberSlotMessages(int slot=0);



		/**
		 * \fn void popSlotMessage(Packet* &data, int slot=0)
		 * \brief Fuehrt net_pop_slot_message auf den Client-Slots
		 * \param data Puffer fuer die zu kopierenden Daten
		 * \param slot Nummer des Slots aus dem ein Paket entnommen wird 
		 * \return Status der Methode
		 *
		 */
		void popSlotMessage( Packet* &data, int slot=0) ;

		/**
		 * \fn void pushSlotMessage( RakNet::BitStream * data,int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE )
		 * \brief Fuehrt net_push_slot_message auf einen Server-Slots aus
		 * \param data Puffer fuer die zu kopierenden Daten
		 * \param slot Slot des Empfaengers
		 * \param prio Prioritaet mit der das Paket gesendet wird
		 * \param reliability Verlaesslichkeit mit der das Paket gesendet wird
		 * \return Status der Methode
		 *
		 */
		void pushSlotMessage( RakNet::BitStream * data, int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE) ;

		/**
		 * \fn int popNewLoginSlot();
		* \brief Prueft ob in login Logindaten vorliegen, sonst gibt die Funktion false zurueck
		* \return Slot auf dem sich ein Spieler eingeloggt hat.
		*/
		int popNewLoginSlot();

		/**
		 * \fn void deallocatePacket(Packet* packet)
		 * \param packet zu loeschendes Paket
		 * \brief Loescht ein nicht mehr benoetigtes Paket. Um Speicherlecks zu vermeiden sollten alle Paket auf mit dieser Funktion entfernt werden
 		*/
		virtual void deallocatePacket(Packet* packet)
		{
#ifndef NO_RAKNET
			m_peer->DeallocatePacket(packet);
#else
			ERRORMSG("Called RakNet lib in NO_RAKNET build");
#endif
		}


	private:

		/**
		 * \fn void pushNewLoginSlot(int slot)
		 * \brief Fuegt einen Slot der Liste der Slots in denen neu angemeldete Clienten warten hinzu
		 */
		void pushNewLoginSlot(int slot);

		/**
		 * \fn unsigned char getPacketIdentifier(Packet *p)
		 * \brief Extrahiert aus einem Paket die Identifikationsnummer
		 */
		unsigned char getPacketIdentifier(Packet *p);

		/**
		 * \fn int insertNewSlot(PlayerID addr)
		 * \brief Fuegt einen neuen Client hinzu
		 * \param addr Netzwerkadresse des Client
		 */
		int insertNewSlot(PlayerID addr);

		/**
		 * \fn int getSlotByAddress(PlayerID address)
		 * \brief ermittelt auf der Netzwerkadresse einen Client den Slot auf dem er angemeldet ist
		 * \param address Netzwerkadresse des Client
		 */
		int getSlotByAddress(PlayerID address);

		/**
		* \var m_active
		* \brief Ist der Server gerade in Betrieb
		*/
		bool m_active;

		/**
		* \var m_slots
		* \brief Die verfuegbaren Slots des Servers
		*/
		NetworkSlot** m_slots;

		/**
		 * \var int m_max_slots
		 * \brief maximale Anzahl Clients die sich anmelden koennen
		 */
        int m_max_slots;




		/**
		 * \var std::queue<int> m_new_login_slots
		 * \brief Warteliste der Clienten die sich schon angemeldet haben, aber noch nicht in die Welt eingefuegt worden sind
		 */
		std::queue<int> m_new_login_slots;


		RakServerInterface* m_peer;
};


#endif

