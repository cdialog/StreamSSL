#pragma once
#include "ISocketStream.h"

/////////////////////////////////////////////////////////////////////////////
// CActiveSock


class CActiveSock : public ISocketStream
{
public:
	explicit CActiveSock(HANDLE StopEvent);
	virtual ~CActiveSock();
	bool Connect(LPCTSTR HostName, USHORT PortNumber);
	void SetRecvTimeoutSeconds(int NewRecvTimeoutSeconds);
	int GetRecvTimeoutSeconds() const;
	void SetSendTimeoutSeconds(int NewSendTimeoutSeconds);
	int GetSendTimeoutSeconds() const;
	// Receives up to Len bytes of data and returns the amount received - or SOCKET_ERROR if it times out
	int RecvMsg(LPVOID lpBuf, const size_t Len);
	// Sends exactly Len bytes of data and returns the amount sent - or SOCKET_ERROR if it times out
	int SendMsg(LPCVOID lpBuf, const size_t Len);
	BOOL ShutDown(int nHow = SD_BOTH);
	// ISocketStream
	DWORD GetLastError() const override;
	int RecvPartial(LPVOID lpBuf, const size_t Len) override;
	// Sends up to Len bytes of data and returns the amount sent - or SOCKET_ERROR if it times out
	int SendPartial(LPCVOID lpBuf, const size_t Len) override;
	// Receives exactly Len bytes of data and returns the amount sent - or SOCKET_ERROR if it times out
	HRESULT Disconnect() override; // Returns S_OK if the close worked

protected:
	SOCKET ActualSocket{ INVALID_SOCKET };
	HANDLE m_hStopEvent{ nullptr };

private:
	bool CloseAndInvalidateSocket();
	static WSADATA WsaData;
	WSAEVENT write_event{ nullptr };
	WSAEVENT read_event{ nullptr };
	WSAOVERLAPPED os{};
	bool RecvInitiated = false;
	DWORD LastError = 0;
	int SendTimeoutSeconds{ 1 }, RecvTimeoutSeconds{ 1 }; // Default timeout is 1 seconds, encourages callers to set it
};