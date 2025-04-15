import 'package:flutter/material.dart';

class TankPainter extends CustomPainter {
  final double level;

  TankPainter({required this.level});

  @override
  void paint(Canvas canvas, Size size) {
    Paint tankPaint = Paint()
      ..color = const Color.fromARGB(255, 132, 136, 139)
      ..style = PaintingStyle.fill;

    Paint waterPaint = Paint()
      ..color = const Color.fromARGB(255, 125, 70, 8)
      ..style = PaintingStyle.fill;

    // Draw the tank
    canvas.drawRRect(
      RRect.fromLTRBR(0, 0, size.width, size.height, const Radius.circular(20)),
      tankPaint,
    );

    // Draw the water level
    double waterHeight = size.height * level;

    canvas.drawRRect(
      RRect.fromLTRBR(0, size.height - waterHeight, size.width, size.height, const Radius.circular(20)),
      waterPaint,
    );

    // Draw the percentage text in the center
    TextSpan span = TextSpan(
      style: const TextStyle(color: Colors.black, fontSize: 20.0, fontWeight: FontWeight.bold),
      text: '${(level * 100).toStringAsFixed(0)}%',
    );

    TextPainter tp = TextPainter(
      text: span,
      textAlign: TextAlign.center,
      textDirection: TextDirection.ltr,
    );

    tp.layout();

    // Calculate the position for the text
    double textX = (size.width - tp.width) / 2;
    double textY = (size.height - tp.height) / 2;

    tp.paint(canvas, Offset(textX, textY));
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) {
    return true;
  }
}