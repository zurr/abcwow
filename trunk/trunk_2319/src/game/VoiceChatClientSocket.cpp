/*
 * Ascent MMORPG Server
 * Voice Chat Engine
 * Copyright (C) 2005-2007 Burlex <burlex@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

VoiceChatClientSocket::VoiceChatClientSocket(uint32 fd) : Socket(fd, 250000, 250000)
{

}

void VoiceChatClientSocket::OnDisconnect()
{
	sVoiceChatHandler.SocketDisconnected();
}

void VoiceChatClientSocket::OnRead()
{
	sVoiceChatHandler.OnRead(GetReadBuffer(0), GetReadBufferSize());
}