
#include "paddle.hpp"
#include<cmath>

Paddle::Paddle(Rectangle bounds) {
	this->bounds = bounds;
	paddleColor = {80, 220, 255, 255}; // cyanish
	speed = defaultSpeed; // Moves across screen in apprx 2.5 s
}

void Paddle::drawPaddle() const
{
	Color bodyColor = paddleColor;

	if(speed > defaultSpeed) {
		drawOverdriveTrail();
		float pulse = 0.85f + 0.15f * sinf(GetTime() * 12.0f);
		bodyColor = {255, (unsigned char)(220 * pulse), 120, 255};
	}
	DrawRectangleRounded(bounds, 0.3f, 16, bodyColor);
	DrawRectangleRounded({bounds.x + 4, bounds.y + 3, bounds.width - 8, 6}, 0.4f, 8, Fade(RAYWHITE, 0.35f)); // Top Highlight
	DrawRectangleRounded({bounds.x + 4, bounds.y + bounds.height - 5, bounds.width - 8, 3}, 0.4f, 8, Fade(BLACK, 0.25f)); // Bottom Shadow

	// Inner-glow during overdrive
	if(speed > defaultSpeed) {
		DrawRectangleRounded({bounds.x + 2, bounds.y + 2, bounds.width - 4, bounds.height - 4}, 0.3f, 16, Fade(RAYWHITE, 0.08f));
	}
	DrawRectangleRoundedLinesEx(bounds, 0.3f, 16, 2.0f, Fade(RAYWHITE, 0.15f));
}

void Paddle::moveLeft(float dt)
{
	/*
	dt is time elapsed since the last frame
	∴ distance to be moved any time key is pressed is = speed * time
	*/
	bounds.x -= speed * dt;

	// Handle boundary condition
	if (bounds.x < 0){
		bounds.x = 0;
	}
}

void Paddle::moveRight(float dt)
{
	/*
	dt is time elapsed since the last frame
	∴ distance to be moved any time key is pressed is = speed * time
	*/
	bounds.x += speed * dt;
	
	// Handle boundary condition
	if (bounds.x + bounds.width > GetScreenWidth()){
		bounds.x = GetScreenWidth() - bounds.width;
	}
}

Color Paddle::getOverdriveColor(float fraction) const
{
	if(fraction < 0.05f) {
		return RED;
	}
	if(fraction < 0.10f) {
		return ORANGE;
	}
	return OVERDRIVE_COLOR;
}

Rectangle Paddle::getBounds() const
{
	return bounds;
}

void Paddle::setSpeed(float speed)
{
	this->speed = speed;
}

void Paddle::activateSpeedBoost(float boost)
{
	speed = defaultSpeed * boost;
}

void Paddle::deactivateSpeedBoost()
{
	speed = defaultSpeed;
}

void Paddle::drawOverdriveEffect(float fraction) const
{
	Vector2 current = {bounds.x, bounds.y};

	drawOverdriveGlow(fraction);
	drawOverdriveBorder(fraction, current);
	drawOverdriveSpark(current, fraction);
}
void Paddle::drawOverdriveGlow(float fraction) const
{
	float pulse = 0.75f + 0.25f * sinf(GetTime() * 10.0f);

	Color glowColor = Fade(getOverdriveColor(fraction), 0.12f * pulse);

	DrawRectangleRounded({bounds.x - 5, bounds.y - 5, bounds.width + 10, bounds.height + 10}, 0.35f, 16, glowColor);

	if(fraction < 0.30f) {
		DrawRectangleRounded({bounds.x - 8, bounds.y - 8, bounds.width + 16, bounds.height + 16}, 0.35f, 16, Fade(RED, 0.08f * pulse));
	}

}

void Paddle::drawOverdriveBorder(float fraction, Vector2 &current) const
{
	// Remaining perimeter length.
	float perimeter = 2.0f * (bounds.width + bounds.height);
	float remaining = perimeter * fraction;

	float top = bounds.width;
	float right = bounds.height;
	float bottom = bounds.width;
	float left = bounds.height;

	float thickness = 4.0f;
	Color energyColor = getOverdriveColor(fraction);

	// Near expiration.
	if(fraction < 0.10f) {
		thickness = 6.0f;
	}
	else if(fraction < 0.20f) {
		thickness = 5.0f;
	}

	// TOP EDGE
	if(remaining > 0) {
		float length = (remaining < top) ? remaining : top;

		DrawLineEx(current, {current.x + length, current.y}, thickness, energyColor);
		current.x += length;
		remaining -= length;
	}

	// RIGHT EDGE
	if(remaining > 0) {
		float length = (remaining < right) ? remaining : right;

		DrawLineEx(current, {current.x, current.y + length}, thickness, energyColor);
		current.y += length;
		remaining -= length;
	}

	// BOTTOM EDGE
	if(remaining > 0) {
		float length = (remaining < bottom) ? remaining : bottom;

		DrawLineEx(current, {current.x - length, current.y}, thickness, energyColor);
		current.x -= length;
		remaining -= length;
	}

	// LEFT EDGE
	if(remaining > 0) {
		float length = (remaining < left) ? remaining : left;

		DrawLineEx(current, {current.x, current.y - length}, thickness, energyColor);
		current.y -= length;
		remaining -= length;
	}

}

void Paddle::drawOverdriveSpark(Vector2 current, float fraction) const
{
	float sparkPulse = 0.7f + 0.3f * sinf(GetTime() * 16.0f);
	float sparkRadius = 5.0f * sparkPulse;

	Color sparkColor = getOverdriveColor(fraction);

	DrawCircleV(current, sparkRadius + 16, Fade(sparkColor, 0.04f));
	DrawCircleV(current, sparkRadius + 10, Fade(sparkColor, 0.10f));
	DrawCircleV(current, sparkRadius + 5, Fade(sparkColor, 0.20f));
	DrawCircleV(current, sparkRadius, sparkColor);
	DrawCircleV(current, 2, RAYWHITE);

	if(fraction < 0.22f) {
		float warning = 0.4f + 0.6f * sinf(GetTime() * 15.0f);
		DrawCircleV(current, sparkRadius + 2, Fade(RED, warning));
	}
}

void Paddle::drawOverdriveTrail() const
{
	float pulse = 0.7f + 0.3f * sinf(GetTime() * 14.0f);
	DrawRectangleRounded({bounds.x - 12, bounds.y + 4, bounds.width * 0.25f, bounds.height - 8}, 0.3f, 16, Fade(OVERDRIVE_COLOR, 0.15f * pulse));
	DrawRectangleRounded({bounds.x - 22, bounds.y + 7, bounds.width * 0.15f, bounds.height - 14}, 0.3f, 16, Fade(OVERDRIVE_COLOR, 0.08f * pulse));
}

void Paddle::reset()
{
	bounds.x = (GetScreenWidth() - bounds.width) / 2.0f;
}

void Paddle::expand()
{
	// Preserve paddle center while increasing width.
	float centerX = bounds.x + bounds.width / 2.0f;

	bounds.width = expandedWidth;
	bounds.x = centerX - bounds.width / 2.0f;
}

void Paddle::resetSize()
{
	// Restore to default paddle width.
	float centerX = bounds.x + bounds.width / 2.0f;

	bounds.width = defaultWidth;
	bounds.x = centerX - bounds.width / 2.0f;
}

bool Paddle::isExpanded() const
{
	return bounds.width == expandedWidth;
}
