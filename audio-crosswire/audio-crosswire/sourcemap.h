struct ChannelSources {
public:
	IMMDevice* m_pChannel0 = NULL;
	IMMDevice* m_pChannel2 = NULL;
	IMMDevice* m_pChannel4 = NULL;
	IMMDevice* m_pChannel6 = NULL;
};

struct SourceTypes {
	EDataFlow m_channel0 = EDataFlow::eCapture;
	EDataFlow m_channel2 = EDataFlow::eCapture;
	EDataFlow m_channel4 = EDataFlow::eCapture;
	EDataFlow m_channel6 = EDataFlow::eCapture;
};