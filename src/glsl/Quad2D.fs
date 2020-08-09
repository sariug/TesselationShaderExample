uniform float lineWidth;
uniform vec3 lineColor;
uniform vec3 quadColor;

noperspective in vec3 EdgeDistance;
out vec4 outColor;


float edgeMix()
{
    // Find the smallest distance
    float d = min( min( EdgeDistance.x, EdgeDistance.y ), EdgeDistance.z );

    if( d < lineWidth - 1 ) {
        return 1.0;
    } else if( d > lineWidth + 1 ) {
        return 0.0;
    } else {
        float x = d - (lineWidth - 1);
        return exp2(-2.0 * (x*x));
    }
}

void main()
{
    float mixVal = edgeMix(); 
    outColor = vec4(mix(quadColor, lineColor, mixVal),1.0f);
}