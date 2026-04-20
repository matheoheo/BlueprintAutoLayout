#include "GraphViewHandler.h"

GraphViewHandler::GraphViewHandler(const sf::Vector2u& size)
{
	mGraphView.setSize({ static_cast<float>(size.x), static_cast<float>(size.y) });
	mGraphView.setCenter(mGraphView.getSize() * 0.5f);
}

void GraphViewHandler::processEvents(const sf::Event& event, const sf::RenderWindow& window)
{
	if (const auto* data = event.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (data->wheel != sf::Mouse::Wheel::Vertical)
			return;
		sf::Vector2f mouseBeforeZoom = window.mapPixelToCoords(sf::Mouse::getPosition(window), mGraphView);

		float zoomFactor = (data->delta > 0) ? 0.9f : 1.1f;
		mGraphView.zoom(zoomFactor);

		sf::Vector2f mouseAfterZoom = window.mapPixelToCoords(sf::Mouse::getPosition(window), mGraphView);
		mGraphView.move(mouseAfterZoom - mouseBeforeZoom);
	}
}

void GraphViewHandler::update(const sf::Time& deltaTime)
{
	constexpr float moveSpeed = 350.f;
	sf::Vector2f dir;

	float currSpeed = moveSpeed * (mGraphView.getSize().x / 1600.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		dir.y = -1.f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		dir.y = 1.f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		dir.x = -1.f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		dir.x = 1.f;

	mGraphView.move(dir * currSpeed * deltaTime.asSeconds());
}

const sf::View& GraphViewHandler::getView() const
{
	return mGraphView;
}
