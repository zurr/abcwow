/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __CHANNEL_H
#define __CHANNEL_H

using namespace std;

enum CHANNEL_FLAGS
{
	CHANNEL_FLAG_NONE				= 0x00,
	CHANNEL_FLAG_OWNER				= 0x01,
	CHANNEL_FLAG_MODERATOR			= 0x02, // "[%s] Moderation privileges given to %s.", "[%s] Moderation privileges removed from %s."
	CHANNEL_FLAG_VOICED				= 0x04,	// "[%s] Chat permission given to %s.", "[%s] %s lost chat permission."
	CHANNEL_FLAG_MUTED				= 0x08,
	CHANNEL_FLAG_CUSTOM				= 0x10,	// "[%s] Voice permission given to %s.", "[%s] %s lost voice permission."
    CHANNEL_FLAG_MICROPHONE_MUTE	= 0x20,
};

enum CHANNEL_NOTIFY_FLAGS
{
	CHANNEL_NOTIFY_FLAG_JOINED		= 0x00, // "%s joined channel."; -- another player joined
	CHANNEL_NOTIFY_FLAG_LEFT		= 0x01, // "%s left channel."; -- another player left
	CHANNEL_NOTIFY_FLAG_YOUJOINED	= 0x02,	// "Joined Channel: [%s]"
	CHANNEL_NOTIFY_FLAG_YOULEFT		= 0x03,	// "Left Channel: [%s]"
	CHANNEL_NOTIFY_FLAG_WRONGPASS	= 0x04,	// "Wrong password for %s."
	CHANNEL_NOTIFY_FLAG_NOTON		= 0x05,	// "Not on channel %s."
	CHANNEL_NOTIFY_FLAG_NOTMOD		= 0x06,	// "Not a moderator of %s."
	CHANNEL_NOTIFY_FLAG_SETPASS		= 0x07,	// "[%s] Password changed by %s."
	CHANNEL_NOTIFY_FLAG_CHGOWNER	= 0x08,	// "[%s] Owner changed to %s."
	CHANNEL_NOTIFY_FLAG_NOT_ON_2	= 0x09,	// "[%s] Player %s was not found."
	CHANNEL_NOTIFY_FLAG_NOT_OWNER	= 0x0A,	// "[%s] You are not the channel owner."
	CHANNEL_NOTIFY_FLAG_WHO_OWNER	= 0x0B,	// "[%s] Channel owner is %s."
	CHANNEL_NOTIFY_FLAG_MODE_CHG	= 0x0C,	// Various (see CHANNEL_FLAGS)
	CHANNEL_NOTIFY_FLAG_ENABLE_ANN	= 0x0D,	// "[%s] Channel announcements enabled by %s."
	CHANNEL_NOTIFY_FLAG_DISABLE_ANN	= 0x0E,	// "[%s] Channel announcements disabled by %s."
	CHANNEL_NOTIFY_FLAG_MODERATED	= 0x0F,	// "[%s] Channel moderation enabled by %s."
	CHANNEL_NOTIFY_FLAG_UNMODERATED	= 0x10,	// "[%s] Channel moderation disabled by %s."
	CHANNEL_NOTIFY_FLAG_YOUCANTSPEAK= 0x11,	// "[%s] You do not have permission to speak."
	CHANNEL_NOTIFY_FLAG_KICKED		= 0x12,	// "[%s] Player %s kicked by %s."
	CHANNEL_NOTIFY_FLAG_YOURBANNED	= 0x13,	// "[%s] You are banned from that channel."
	CHANNEL_NOTIFY_FLAG_BANNED		= 0x14,	// "[%s] Player %s banned by %s."
	CHANNEL_NOTIFY_FLAG_UNBANNED	= 0x15,	// "[%s] Player %s unbanned by %s."
	CHANNEL_NOTIFY_FLAG_UNK_1		= 0x16,	// "[%s] Player %s is not banned."
	CHANNEL_NOTIFY_FLAG_ALREADY_ON	= 0x17,	// "[%s] Player %s is already on the channel."
	CHANNEL_NOTIFY_FLAG_INVITED		= 0x18,	// "%2$s has invited you to join the channel '%1$s'."
	CHANNEL_NOTIFY_FLAG_WRONG_FACT	= 0x19,	// "Target is in the wrong alliance for %s."
	CHANNEL_NOTIFY_FLAG_UNK_2		= 0x1A,	// "Wrong alliance for %s."
	CHANNEL_NOTIFY_FLAG_UNK_3		= 0x1B,	// "Invalid channel name"
	CHANNEL_NOTIFY_FLAG_UNK_4		= 0x1C,	// "%s is not moderated"
	CHANNEL_NOTIFY_FLAG_YOU_INVITED	= 0x1D,	// "[%s] You invited %s to join the channel"
	CHANNEL_NOTIFY_FLAG_UNK_5		= 0x1E,	// "[%s] %s has been banned."
	CHANNEL_NOTIFY_FLAG_UNK_6		= 0x1F,	// "[%s] The number of messages that can be sent to this channel is limited, please wait to send another message."
	CHANNEL_NOTIFY_FLAG_UNK_7		= 0x20,	// "[%s] You are not in the correct area for this channel."; -- The user is trying to send a chat to a zone specific channel, and they're not physically in that zone.
	CHANNEL_NOTIFY_FLAG_NOT_IN_LFG	= 0x21,	// "[%s] You must be queued in looking for group before joining this channel."; -- The user must be in the looking for group system to join LFG chat channels.
	CHANNEL_NOTIFY_FLAG_VOICE_ON	= 0x22,	// "[%s] Channel voice enabled by %s."
	CHANNEL_NOTIFY_FLAG_VOICE_OFF	= 0x23, // "[%s] Channel voice disabled by %s."
};

class Channel
{
	Mutex m_lock;
	typedef map<Player*, uint32> MemberMap;
	MemberMap m_members;
	set<uint32> m_bannedMembers;
public:
	friend class ChannelIterator;
	static void LoadConfSettings();
	string m_name;
	string m_password;
	uint8 m_flags;
	uint32 m_id;
	bool m_general;
	bool m_muted;
	bool m_announce;
	uint32 m_team;
	ARCEMU_INLINE size_t GetNumMembers() { return m_members.size(); }
#ifdef VOICE_CHAT
	bool voice_enabled;
	uint16 i_voice_channel_id;
	MemberMap m_VoiceMembers;
#endif
	uint32 m_minimumLevel;
public:
	Channel(const char * name, uint32 team, uint32 type_id);
	~Channel();

	void AttemptJoin(Player * plr, const char * password);
	void Part(Player * plr, bool send_packet = true);
	void Kick(Player * plr, Player * die_player, bool ban);
	void Invite(Player * plr, Player * new_player);
	void Moderate(Player * plr);
	void Mute(Player * plr, Player * die_player);
	void Voice(Player * plr, Player * v_player);
	void Unmute(Player * plr, Player * die_player);
	void Devoice(Player * plr, Player * v_player);
	void Say(Player * plr, const char * message, Player * for_gm_client, bool forced);
	void Unban(Player * plr, PlayerInfo * bplr);
	void GiveModerator(Player * plr, Player * new_player);
	void TakeModerator(Player * plr, Player * new_player);
	void Announce(Player * plr);
	void Password(Player * plr, const char * pass);
	void List(Player * plr);
	void GetOwner(Player * plr);

	void SetOwner(Player * oldpl, Player * plr);

	// Packet Forging
	void SendAlreadyOn(Player * plr, Player * plr2);
	void SendYouAreBanned(Player * plr);
	void SendNotOn(Player * plr);
	void SendNotOwner(Player * plr);
	void SendYouCantSpeak(Player * plr);
	void SendModeChange(Player * plr, uint8 old_flags, uint8 new_flags);

	void SendToAll(WorldPacket * data);
	void SendToAll(WorldPacket * data, Player * plr);

#ifdef VOICE_CHAT
	void VoiceChannelCreated(uint16 id);
	void JoinVoiceChannel(Player * plr);
	void PartVoiceChannel(Player * plr);
	void SendVoiceUpdate();
	void VoiceDied();

#endif

	bool HasMember(Player * pPlayer);
};

class ChannelIterator
{
	Channel::MemberMap::iterator m_itr;
	Channel::MemberMap::iterator m_endItr;
	bool m_searchInProgress;
	Channel * m_target;
public:
	ChannelIterator(Channel* target) : m_searchInProgress(false),m_target(target) {}
	~ChannelIterator() { if(m_searchInProgress) { EndSearch(); } }

	void BeginSearch()
	{
		// iteminterface doesn't use mutexes, maybe it should :P
		ASSERT(!m_searchInProgress);
		m_target->m_lock.Acquire();
		m_itr = m_target->m_members.begin();
		m_endItr = m_target->m_members.end();
		m_searchInProgress=true;
	}

	void EndSearch()
	{
		// nothing here either
		ASSERT(m_searchInProgress);
		m_target->m_lock.Release();
		m_searchInProgress=false;
	}

	Player* operator*() const
	{
		return m_itr->first;
	}

	Player* operator->() const
	{
		return m_itr->first;
	}

	void Increment()
	{
		if(!m_searchInProgress)
			BeginSearch();

		if(m_itr==m_endItr)
			return;

		++m_itr;
	}

	ARCEMU_INLINE Player* Grab() { return m_itr->first; }
	ARCEMU_INLINE bool End() { return (m_itr==m_endItr)?true:false; }
};

#endif
