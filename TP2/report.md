# Report

## 3.2.2

1. The Faces are filled with Solid. The shadows are missing.
2. The Sphere has more/less vertices in on the meridians/parallels when changing the two parameters.

## 3.2.3

1. The view streches in order to fit the image when we resize the window. To preserve the aspect ratio, we have to always have the same proportions between the height and width.

## 4.2.1

1. The field of view is changed, that means we either see the teapot bigger or smaller, because the angle is bigger/smaller and that make the picture appear more zoomed in/out.
2. When changing zNear, the teapot get cut with zNear > 2.1.

## 4.2.2

1. The teapot appears its head down.
2. In the first view, to get the right up vector, we put it along the x-axis to fit the "teapot axis". In the second one, we have to put two fields to ±1.0, so that the angle is 45°.

## 4.3.2

1. Each time the display function is called, a new matrix is added at the top of the stack, equalling the previous top one multiplied by glTranslate(0, -2, -1). This means each time the teapots are drawn, they are translated another time.
2. If gluLookAt is called after the 2nd Push, the blue teapot disappears. What happens is that red and green teapots are rendered correctly according to the camera location and orientation set by gluLookAt (they are in front of the camera), but the blue one is rendered according to the default modelview matrix, which is out of the view. In the end, only the green and red ones are visible. In the stack, only the top matrix gets multiplied by gluLookAt and is correctly set up.
3. In this situation, the gluLookAt matrix multiplication done in main is overriden by the identity matrix for each display call, so we do not place the teapots so that they all can be seen by that default camera which is closer to the teapots.


