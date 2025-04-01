#pragma once
#include "ParameterGroup.h"
#include "ParameterBase.h"
#include <QtPlugin>
#include <QMetaType>
#include <QList>

class IVideoProcessorConfigurable
{
public:
	enum class OverlayTimecode
	{
		None,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

public:
	virtual ~IVideoProcessorConfigurable() = default;

	virtual QList<const ParameterGroup*> getGroups() const = 0;
	virtual QList<const ParameterBase*> getParameters() const = 0;
	virtual bool saveParameters() const = 0;
	virtual void resetParameters() = 0;
};

Q_DECLARE_METATYPE(IVideoProcessorConfigurable::OverlayTimecode)

#define IVideoProcessorConfigurable_iid "com.surveilclip.IVideoProcessorConfigurable/1.0"
Q_DECLARE_INTERFACE(IVideoProcessorConfigurable, IVideoProcessorConfigurable_iid)
