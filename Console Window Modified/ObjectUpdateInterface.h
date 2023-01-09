// -----------------------------------------------------------------------------
#ifndef ObjectUpdateInterface_H
#define ObjectUpdateInterface_H
// -----------------------------------------------------------------------------

//--INCLUDES--//


// -----------------------------------------------------------------------------

template<typename RenderWindowType, typename TickType>
class ObjectUpdateInterface
{
public:
	virtual ~ObjectUpdateInterface() {}

	virtual void render(RenderWindowType& pWindow) = 0;
	virtual void update(const TickType& pDeltaTime) = 0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !ObjectUpdateInterface_H
